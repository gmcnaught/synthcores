#!/usr/bin/env python3
# SPDX-License-Identifier: MIT
"""Load and save SysEx dumps through a core's MIDI UART (on the MiSTer: /dev/ttyS1).

  syx.py info FILE...                         list the SysEx messages in .syx / .mid files
  syx.py send FILE... [options]               send them to the core, paced for the receiver
  syx.py dump OUT.syx REQUEST_HEX... [options]
                                              send requests, write the SysEx the core sends back
  syx.py dump OUT.syx --cz PP [options]       Casio CZ: fetch tone PP over the CZ handshake

Options: --dev DEV (/dev/ttyS1), --baud N (31250; 0 leaves the port's speed alone),
--gap MS (pause after each message; default per maker, see GAP_MS), --timeout S (replies,
default 2), --idle MS (dump: stop after this long without a byte, default 1000),
--channel N (CZ MIDI channel 1-16, default 1), --dry-run (send: print the plan only),
--stop-midilink (stop MidiLink and the uartmode loop that respawns it; they hold the port).

Files: a .syx file is a byte stream of F0 ... F7 messages (other bytes between them are
skipped with a note). A .mid file (SMF 0/1) gives its F0 events, with F7 continuation
packets joined. Each message is sent whole, then the tty is drained and GAP_MS waited.

Receivers that answer (the protocol decides, not a timer):
- Casio CZ (F0 44 00 00 7n 20 pp + 256 data + F7): the 7-byte header goes first, the CZ answers
  F0 44 00 00 7n 30, then the data and F7 go, and the CZ answers F7. `dump --cz PP` runs the
  send request (10 pp, answer 30, 7n 31, data + F7, F7) and writes the tone in the 20 pp form,
  so the file can be sent back.
- E-mu Proteus 2000 family (F0 18 0F dd 55 ...): closed-loop dump messages (header 10 01,
  data 10 02) wait for the ACK (55 7F pp pp) through WAITs (55 7C); a NAK (55 7E) sends the packet
  again (up to 3 times), a CANCEL (55 7D) stops. `dump` ACKs each closed-loop packet it receives.
Every reply the core sends is printed.
"""
import argparse
import os
import select
import struct
import sys
import time

# ---------------------------------------------------------------- parsing

def split_stream(data, notes=None):
    """Raw MIDI bytes -> list of complete F0..F7 messages. Realtime bytes (F8-FF) inside a
    message are dropped, as a receiver would; a status byte that ends a message early drops it."""
    msgs, cur, stray = [], None, 0
    for b in data:
        if b >= 0xF8:
            continue
        if b == 0xF0:
            if cur is not None and notes is not None:
                notes.append(f"message {len(msgs)} has no F7 ({len(cur)} bytes): dropped")
            cur = bytearray([b])
        elif cur is not None:
            if b == 0xF7:
                cur.append(b)
                msgs.append(bytes(cur))
                cur = None
            elif b & 0x80:
                if notes is not None:
                    notes.append(f"message {len(msgs)} cut by status {b:02X}: dropped")
                cur = None
            else:
                cur.append(b)
        else:
            stray += 1
    if cur is not None and notes is not None:
        notes.append(f"last message has no F7 ({len(cur)} bytes): dropped")
    if stray and notes is not None:
        notes.append(f"{stray} bytes outside SysEx messages skipped")
    return msgs


def _vlq(d, i):
    v = 0
    while True:
        b = d[i]
        i += 1
        v = (v << 7) | (b & 0x7F)
        if not b & 0x80:
            return v, i


def smf_sysex(data, notes=None):
    """Standard MIDI file -> SysEx messages, in file order per track, tracks in order."""
    if data[:4] != b"MThd":
        raise ValueError("not a standard MIDI file")
    hlen = struct.unpack(">I", data[4:8])[0]
    pos, out = 8 + hlen, bytearray()
    while pos + 8 <= len(data):
        cid, clen = data[pos:pos + 4], struct.unpack(">I", data[pos + 4:pos + 8])[0]
        trk, pos = data[pos + 8:pos + 8 + clen], pos + 8 + clen
        if cid != b"MTrk":
            continue
        i, running, open_sx = 0, 0, False
        while i < len(trk):
            _, i = _vlq(trk, i)
            st = trk[i]
            if st == 0xFF:
                ln, j = _vlq(trk, i + 2)
                i = j + ln
            elif st in (0xF0, 0xF7):
                ln, j = _vlq(trk, i + 1)
                body = trk[j:j + ln]
                i = j + ln
                if st == 0xF0:
                    out += b"\xF0" + body
                    open_sx = not body.endswith(b"\xF7")
                elif open_sx:          # continuation packet
                    out += body
                    open_sx = not body.endswith(b"\xF7")
                else:                  # escape: raw bytes
                    out += body
            else:
                if st & 0x80:
                    running, i = st, i + 1
                elif not running:
                    raise ValueError("running status without a status byte")
                i += 2 if (running & 0xF0) not in (0xC0, 0xD0) else 1
    return split_stream(bytes(out), notes)


def load(path, notes=None):
    with open(path, "rb") as f:
        data = f.read()
    if data[:4] == b"MThd":
        return smf_sysex(data, notes)
    if data.lstrip()[:1] == b"<" and notes is not None:
        notes.append("this is an HTML page, not a dump (a download that needed a login?)")
    return split_stream(data, notes)

# ---------------------------------------------------------------- what a message is

GAP_MS = {      # pause after each message (after the tty drains), by maker ID
    0x41: 40,   # Roland: DT1 needs >= 20 ms between packets (JV-880, MT-32, SC-55 manuals)
    0x18: 60,   # E-mu: Proteus/1 preset messages write battery RAM; P2K open-loop dumps
    0x44: 20,   # Casio: the CZ handshake paces the data itself
    0x43: 100,  # Yamaha bulk dumps
}
GAP_DEFAULT = 60


def roland_checksum_ok(m):
    """Roland DT1/RQ1: address + data + checksum sum to 0 mod 128."""
    return len(m) >= 7 and sum(m[5:-1]) & 0x7F == 0


def describe(m):
    if len(m) < 3:
        return "short"
    if m[1] == 0x7E or m[1] == 0x7F:
        return f"universal {'non-' if m[1] == 0x7E else ''}realtime {m[3]:02X}" if len(m) > 4 else "universal"
    if m[1] == 0x41 and len(m) >= 6:
        model = {0x16: "LA (MT-32)", 0x42: "GS (SC-55)", 0x45: "SC-55 display", 0x46: "JV-80/880"}
        cmd = {0x11: "RQ1", 0x12: "DT1"}.get(m[4], f"cmd {m[4]:02X}")
        s = f"Roland dev {m[2]:02X} {model.get(m[3], f'model {m[3]:02X}')} {cmd}"
        if m[4] in (0x11, 0x12) and len(m) >= 10:
            s += f" addr {m[5]:02X} {m[6]:02X} {m[7]:02X}"
            if m[3] == 0x46:
                s += f" {m[8]:02X}"
            s += "" if roland_checksum_ok(m) else " BAD CHECKSUM"
        return s
    if m[1] == 0x44 and len(m) >= 7 and m[2:4] == b"\0\0" and m[4] & 0xF0 == 0x70:
        cmd = {0x10: "send request", 0x20: "tone data", 0x30: "ack", 0x31: "go"}.get(m[5], f"cmd {m[5]:02X}")
        return f"Casio CZ ch {(m[4] & 15) + 1} {cmd} {m[6]:02X}"
    if m[1] == 0x18 and len(m) >= 5:
        if m[2] == 0x04:
            cmd = {0: "preset request", 1: "preset data", 3: "parameter", 4: "tuning", 6: "program map",
                   8: "master settings"}.get(m[4], f"cmd {m[4]:02X}")
            s = f"E-mu Proteus/1 dev {m[3]:02X} {cmd}"
            if m[4] in (0, 1) and len(m) >= 8:
                s += f" preset {m[5] | m[6] << 7}"
            return s
        if m[2] == 0x0F and len(m) >= 6:
            cmd = {(0x10, 0x01): "preset dump header (closed)", (0x10, 0x02): "preset dump data (closed)",
                   (0x10, 0x03): "preset dump header (open)", (0x10, 0x04): "preset dump data (open)",
                   (0x11, 0x02): "preset dump request (closed)", (0x11, 0x04): "preset dump request (open)",
                   (0x7F,): "ACK", (0x7E,): "NAK", (0x7D,): "CANCEL", (0x7C,): "WAIT", (0x7B,): "EOF"}
            key = (m[5], m[6]) if len(m) > 6 else (m[5],)
            return f"E-mu P2K family dev {m[3]:02X} {cmd.get(key, cmd.get((m[5],), f'cmd {m[5]:02X}'))}"
        return f"E-mu model {m[2]:02X}"
    return f"maker {m[1]:02X}"


def gap_for(m, override):
    if override is not None:
        return override / 1000
    return GAP_MS.get(m[1] if len(m) > 1 else 0, GAP_DEFAULT) / 1000


def is_cz_tone(m):
    return len(m) > 8 and m[1] == 0x44 and m[2:4] == b"\0\0" and m[4] & 0xF0 == 0x70 and m[5] == 0x20


def emu_closed_loop(m):
    return len(m) > 7 and m[1] == 0x18 and m[2] == 0x0F and m[5] == 0x10 and m[6] in (0x01, 0x02)

# ---------------------------------------------------------------- the port

TCGETS2, TCSETS2, CBAUD, BOTHER = 0x802C542A, 0x402C542B, 0o010017, 0o010000


class Port:
    def __init__(self, dev, baud):
        self.fd = os.open(dev, os.O_RDWR | os.O_NOCTTY)
        import termios
        self.termios = termios
        if os.isatty(self.fd):
            a = termios.tcgetattr(self.fd)
            a[0] = a[1] = a[3] = 0
            a[2] = termios.CS8 | termios.CREAD | termios.CLOCAL
            a[6][termios.VMIN] = 0
            a[6][termios.VTIME] = 0
            termios.tcsetattr(self.fd, termios.TCSANOW, a)
        if baud:
            import fcntl
            t2 = bytearray(44)          # struct termios2, 32-bit ARM (the MiSTer's HPS)
            fcntl.ioctl(self.fd, TCGETS2, t2)
            i, o, c, l = struct.unpack_from("4I", t2)
            struct.pack_into("4I", t2, 0, i, o, (c & ~CBAUD) | BOTHER, l)
            struct.pack_into("2I", t2, 36, baud, baud)
            fcntl.ioctl(self.fd, TCSETS2, t2)
        if os.isatty(self.fd):
            termios.tcflush(self.fd, termios.TCIFLUSH)
        self.rx = bytearray()           # bytes read, not yet taken by a parser

    def write(self, data):
        view = memoryview(data)
        while view:
            n = os.write(self.fd, view)
            view = view[n:]
        if os.isatty(self.fd):
            self.termios.tcdrain(self.fd)   # gaps count from the last stop bit

    def read_some(self, timeout):
        r, _, _ = select.select([self.fd], [], [], max(0.0, timeout))
        if r:
            got = os.read(self.fd, 4096)
            self.rx += got
            return len(got)
        return 0

    def next_message(self, timeout):
        """The next complete F0..F7 message from the core (realtime and other bytes dropped),
        or None after timeout seconds. Bytes after it stay buffered."""
        end = time.monotonic() + timeout
        while True:
            start = self.rx.find(0xF0)
            if start < 0:
                self.rx.clear()
            else:
                del self.rx[:start]
                stop = self.rx.find(0xF7)
                if stop >= 0:
                    m = bytes(b for b in self.rx[:stop + 1] if b < 0xF8)
                    del self.rx[:stop + 1]
                    return m
            if not self.read_some(end - time.monotonic()) and time.monotonic() >= end:
                return None

    def until(self, pattern, timeout):
        """Read until the raw byte string `pattern` arrives (realtime bytes ignored); True if it did."""
        end = time.monotonic() + timeout
        while True:
            clean = bytes(b for b in self.rx if b < 0xF8)
            k = clean.find(pattern)
            if k >= 0:
                self.rx = bytearray(clean[k + len(pattern):])
                return True
            if not self.read_some(end - time.monotonic()) and time.monotonic() >= end:
                return False

    def drain_replies(self, wait):
        """Messages the core sends within `wait` seconds."""
        out, end = [], time.monotonic() + wait
        while True:
            m = self.next_message(end - time.monotonic())
            if m is None:
                return out
            out.append(m)

# ---------------------------------------------------------------- commands

def stop_midilink():
    me = os.getpid()
    for p in os.listdir("/proc"):
        if not p.isdigit() or int(p) == me:
            continue
        try:
            with open(f"/proc/{p}/cmdline", "rb") as f:
                cmd = f.read().replace(b"\0", b" ")
        except OSError:
            continue
        if b"/sbin/uartmode" in cmd or cmd.split(b" ")[0].endswith(b"midilink"):
            try:
                os.kill(int(p), 15)
            except OSError:
                pass
    time.sleep(1)


def midilink_running():
    for p in os.listdir("/proc"):
        if p.isdigit():
            try:
                with open(f"/proc/{p}/comm") as f:
                    if f.read().strip() == "midilink":
                        return True
            except OSError:
                pass
    return False


def hx(b, n=12):
    s = b[:n].hex(" ").upper()
    return s + (f" ... ({len(b)} bytes)" if len(b) > n else "")


def cmd_info(a):
    total = 0
    for path in a.files:
        notes = []
        msgs = load(path, notes)
        total += len(msgs)
        print(f"{path}: {len(msgs)} messages, {sum(map(len, msgs))} bytes")
        for k, m in enumerate(msgs):
            print(f"  {k:4} {len(m):6}  {describe(m)}")
        for n in notes:
            print(f"  note: {n}")
    return 0 if total else 1


def send_cz(port, m, a):
    head, body = m[:7], m[7:]
    ack = bytes([0xF0, 0x44, 0, 0, m[4], 0x30])
    port.write(head)
    if not port.until(ack, a.timeout):
        print("  CZ did not answer the tone header (F0 44 00 00 7n 30); is it on that MIDI channel?")
        return False
    port.write(body)
    if not port.until(b"\xF7", a.timeout):
        print("  CZ did not end the transfer with F7")
        return False
    return True


def send_emu_closed(port, m, a):
    for attempt in range(3):
        port.write(m)
        while True:
            r = port.next_message(a.timeout)
            if r is None:
                print("  no ACK from the E-mu unit")
                return False
            if len(r) > 5 and r[1] == 0x18 and r[2] == 0x0F and r[5] == 0x7F:
                if a.verbose:
                    print(f"  ACK {hx(r)}")
                return True
            if len(r) > 5 and r[1] == 0x18 and r[2] == 0x0F and r[5] == 0x7D:
                print("  CANCEL from the E-mu unit")
                return False
            if len(r) > 5 and r[1] == 0x18 and r[2] == 0x0F and r[5] == 0x7C:
                continue                # WAIT: the unit is busy (seen after every packet on the P2K)
            if len(r) > 5 and r[1] == 0x18 and r[2] == 0x0F and r[5] == 0x7E:
                print(f"  NAK, sending again ({attempt + 1})")
                break
            print(f"  reply: {hx(r)}")
    return False


def cmd_send(a):
    msgs, notes = [], []
    for path in a.files:
        msgs += load(path, notes)
    for n in notes:
        print(f"note: {n}")
    if not msgs:
        print("no SysEx messages")
        return 1
    bad = [k for k, m in enumerate(msgs) if len(m) > 6 and m[1] == 0x41 and m[4] == 0x12 and not roland_checksum_ok(m)]
    if bad:
        print(f"Roland checksum wrong in messages {bad[:10]}; not sending")
        return 1
    secs = sum(len(m) * 10 / 31250 + (0 if is_cz_tone(m) else gap_for(m, a.gap)) for m in msgs)
    print(f"{len(msgs)} messages, {sum(map(len, msgs))} bytes, about {secs:.1f} s")
    if a.dry_run:
        for k, m in enumerate(msgs):
            how = "CZ handshake" if is_cz_tone(m) else "wait ACK" if emu_closed_loop(m) else f"gap {gap_for(m, a.gap) * 1000:.0f} ms"
            print(f"  {k:4} {len(m):6}  {describe(m)}  [{how}]")
        return 0
    if a.stop_midilink:
        stop_midilink()
    elif os.path.exists("/proc/self") and midilink_running():
        print("MidiLink is running and holds the port: use --stop-midilink (or synth-mister midi-off)")
        return 1
    port = Port(a.dev, a.baud)
    failed = 0
    for k, m in enumerate(msgs):
        if is_cz_tone(m):
            ok = send_cz(port, m, a)
        elif emu_closed_loop(m):
            ok = send_emu_closed(port, m, a)
        else:
            port.write(m)
            ok = True
            for r in port.drain_replies(gap_for(m, a.gap)):
                print(f"  {k}: reply {hx(r)}  {describe(r)}")
        if not ok:
            failed += 1
            print(f"  message {k} ({describe(m)}) failed")
            if a.stop_on_error:
                return 1
        if a.verbose or (k + 1) % 50 == 0 or k + 1 == len(msgs):
            print(f"  sent {k + 1}/{len(msgs)}")
    for r in port.drain_replies(0.3):
        print(f"  reply {hx(r)}  {describe(r)}")
    print(f"done: {len(msgs) - failed} sent, {failed} failed")
    return 1 if failed else 0


def cmd_dump(a):
    if a.stop_midilink:
        stop_midilink()
    port = Port(a.dev, a.baud)
    got = []
    if a.cz is not None:
        n = 0x70 | (a.channel - 1)
        pp = int(a.cz, 16)
        port.write(bytes([0xF0, 0x44, 0, 0, n, 0x10, pp]))
        if not port.until(bytes([0xF0, 0x44, 0, 0, n, 0x30]), a.timeout):
            print("CZ did not answer the send request")
            return 1
        port.write(bytes([n, 0x31]))
        end = time.monotonic() + a.timeout + 2
        data = bytearray()
        while time.monotonic() < end:
            port.read_some(0.1)
            clean = bytes(b for b in port.rx if b < 0xF8)
            k = clean.find(0xF7)
            if k >= 0:
                data = clean[:k]
                break
        else:
            print(f"CZ data did not end with F7 ({len(port.rx)} bytes)")
            return 1
        port.write(b"\xF7")
        got.append(bytes([0xF0, 0x44, 0, 0, n, 0x20, pp]) + bytes(data) + b"\xF7")
    else:
        for h in a.requests:
            port.write(bytes.fromhex(h))
            # Some units start after a pause (Proteus/1: "SENDING DATA..." on the LCD)
            m = port.next_message(a.timeout)
            if m is None:
                print(f"no reply to {h}")
                continue
            while m is not None:
                if len(m) > 5 and m[1] == 0x18 and m[2] == 0x0F and m[4] == 0x55 and m[5] in (0x7C, 0x7B):
                    pass                # WAIT / EOF: handshake, not data
                else:
                    got.append(m)
                if emu_closed_loop(m):  # the unit waits for our ACK of each packet (header: 0)
                    pkt = m[7:9] if m[6] == 0x02 else b"\0\0"
                    port.write(bytes([0xF0, 0x18, 0x0F, m[3], 0x55, 0x7F]) + pkt + b"\xF7")
                m = port.next_message(a.idle / 1000)
    with open(a.out, "wb") as f:
        for m in got:
            f.write(m)
    print(f"{a.out}: {len(got)} messages, {sum(map(len, got))} bytes")
    for k, m in enumerate(got[:a.list]):
        print(f"  {k:4} {len(m):6}  {describe(m)}")
    return 0 if got else 1


def main(argv=None):
    p = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    sub = p.add_subparsers(dest="cmd", required=True)

    def port_opts(q):
        q.add_argument("--dev", default="/dev/ttyS1")
        q.add_argument("--baud", type=int, default=31250)
        q.add_argument("--timeout", type=float, default=2.0)
        q.add_argument("--channel", type=int, default=1)
        q.add_argument("--stop-midilink", action="store_true")

    q = sub.add_parser("info")
    q.add_argument("files", nargs="+")
    q = sub.add_parser("send")
    q.add_argument("files", nargs="+")
    port_opts(q)
    q.add_argument("--gap", type=float)
    q.add_argument("--dry-run", action="store_true")
    q.add_argument("--stop-on-error", action="store_true")
    q.add_argument("-v", "--verbose", action="store_true")
    q = sub.add_parser("dump")
    q.add_argument("out")
    q.add_argument("requests", nargs="*")
    port_opts(q)
    q.add_argument("--cz", metavar="PP")
    q.add_argument("--idle", type=float, default=1000)
    q.add_argument("--list", type=int, default=20)
    a = p.parse_args(argv)
    if a.cmd == "dump" and not a.requests and a.cz is None:
        p.error("dump needs REQUEST_HEX or --cz PP")
    return {"info": cmd_info, "send": cmd_send, "dump": cmd_dump}[a.cmd](a)


if __name__ == "__main__":
    sys.exit(main())
