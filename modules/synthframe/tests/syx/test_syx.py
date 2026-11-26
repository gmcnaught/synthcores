#!/usr/bin/env python3
# SPDX-License-Identifier: MIT
"""Unit tests for tools/syx.py: file parsing, and the send/dump protocols against a fake
device on a pty (no MiSTer needed). Run: python3 modules/synthframe/tests/syx/test_syx.py"""
import io
import os
import struct
import sys
import tempfile
import threading
import time
import unittest
from contextlib import redirect_stdout

sys.path.insert(0, os.path.join(os.path.dirname(__file__), "../../tools"))
import syx  # noqa: E402


def roland_dt1(dev, model, addr, data):
    body = bytes(addr) + bytes(data)
    return bytes([0xF0, 0x41, dev, model, 0x12]) + body + bytes([(-sum(body)) & 0x7F, 0xF7])


def vlq(n):
    out = [n & 0x7F]
    n >>= 7
    while n:
        out.insert(0, 0x80 | (n & 0x7F))
        n >>= 7
    return bytes(out)


def smf(events):
    trk = b"".join(events) + b"\x00\xFF\x2F\x00"
    return b"MThd" + struct.pack(">IHHH", 6, 0, 1, 96) + b"MTrk" + struct.pack(">I", len(trk)) + trk


class Device(threading.Thread):
    """The far end of a pty: records what arrives, answers through `respond(rx_so_far, fd)`."""

    def __init__(self, master, respond=None):
        super().__init__(daemon=True)
        self.master, self.respond, self.rx, self.stop = master, respond, bytearray(), False

    def run(self):
        import select
        while not self.stop:
            r, _, _ = select.select([self.master], [], [], 0.02)
            if r:
                try:
                    got = os.read(self.master, 4096)
                except OSError:
                    return
                self.rx += got
                if self.respond:
                    self.respond(self, got)


def pty_pair():
    import tty
    m, s = os.openpty()
    tty.setraw(m)
    name = os.ttyname(s)
    return m, s, name


def read(path):
    with open(path, "rb") as f:
        return f.read()


def run(argv):
    buf = io.StringIO()
    with redirect_stdout(buf):
        rc = syx.main(argv)
    return rc, buf.getvalue()


class Parse(unittest.TestCase):
    def test_stream(self):
        a = roland_dt1(0x10, 0x46, [0, 0, 0, 0], [1, 2, 3])
        data = b"\x90\x3c\x40" + a[:5] + b"\xF8\xFE" + a[5:] + b"\xF0\x44\x00" + b"\xC0\x01" + b"\xF0\x18\x04\x00\x03\x28\x00\x01\x00\xF7"
        notes = []
        msgs = syx.split_stream(data, notes)
        self.assertEqual(msgs, [a, b"\xF0\x18\x04\x00\x03\x28\x00\x01\x00\xF7"])
        self.assertTrue(any("cut by status C0" in n for n in notes))
        self.assertTrue(any("outside SysEx" in n for n in notes))

    def test_smf(self):
        a = roland_dt1(0x10, 0x46, [1, 0x40, 0x20, 0], list(range(20)))
        ev = [b"\x00\x90\x3c\x40", b"\x10\x3e\x40",  # running status
              b"\x00\xFF\x03\x02ab",                 # meta
              b"\x00\xF0" + vlq(10) + a[1:11],       # first packet, no F7
              b"\x05\xF7" + vlq(len(a) - 11) + a[11:],  # continuation, ends with F7
              b"\x00\xF0" + vlq(5) + b"\x7E\x7F\x09\x01\xF7",
              b"\x00\xC0\x05"]
        with tempfile.NamedTemporaryFile(suffix=".mid", delete=False) as f:
            f.write(smf(ev))
        try:
            self.assertEqual(syx.load(f.name), [a, b"\xF0\x7E\x7F\x09\x01\xF7"])
        finally:
            os.unlink(f.name)

    def test_describe(self):
        a = roland_dt1(0x10, 0x46, [1, 0x40, 0x20, 0], [1, 2])
        self.assertIn("JV-80/880 DT1 addr 01 40 20 00", syx.describe(a))
        self.assertNotIn("BAD", syx.describe(a))
        bad = a[:-2] + bytes([(a[-2] + 1) & 0x7F, 0xF7])
        self.assertIn("BAD CHECKSUM", syx.describe(bad))
        self.assertIn("Proteus/1 dev 00 preset data preset 64", syx.describe(b"\xF0\x18\x04\x00\x01\x40\x00\x01\xF7"))


class Protocol(unittest.TestCase):
    def setUp(self):
        self.m, self.s, self.name = pty_pair()
        self.dir = tempfile.mkdtemp()

    def tearDown(self):
        if hasattr(self, "dev"):
            self.dev.stop = True
            self.dev.join(1)
        os.close(self.s)
        os.close(self.m)

    def start(self, respond=None):
        self.dev = Device(self.m, respond)
        self.dev.start()

    def file(self, name, data):
        p = os.path.join(self.dir, name)
        with open(p, "wb") as f:
            f.write(data)
        return p

    def test_plain_with_gap(self):
        msgs = [roland_dt1(0x10, 0x46, [1, 0x40, k, 0], [k] * 30) for k in range(5)]
        p = self.file("a.syx", b"".join(msgs))
        self.start()
        t0 = time.monotonic()
        rc, out = run(["send", p, "--dev", self.name, "--baud", "0", "--gap", "50"])
        took = time.monotonic() - t0
        time.sleep(0.1)
        self.assertEqual(rc, 0, out)
        self.assertEqual(bytes(self.dev.rx), b"".join(msgs))
        self.assertGreaterEqual(took, 0.25)

    def test_roland_bad_checksum_refused(self):
        a = roland_dt1(0x10, 0x46, [1, 0x40, 0, 0], [1, 2])
        p = self.file("b.syx", a[:-2] + bytes([(a[-2] + 1) & 0x7F, 0xF7]))
        self.start()
        rc, out = run(["send", p, "--dev", self.name, "--baud", "0"])
        self.assertEqual(rc, 1)
        self.assertIn("checksum", out)
        self.assertEqual(self.dev.rx, b"")

    def test_cz_send(self):
        data = bytes((k * 7) & 0x0F for k in range(256))
        tone = bytes([0xF0, 0x44, 0, 0, 0x70, 0x20, 0x21]) + data + b"\xF7"
        p = self.file("cz.syx", tone)
        seen = []

        def cz(dev, got):
            if dev.rx == tone[:7] and not seen:
                seen.append("head")
                os.write(dev.master, b"\xFE\xF0\x44\x00\x00\x70\x30")
            elif dev.rx.endswith(b"\xF7") and len(dev.rx) == len(tone):
                seen.append("body")
                os.write(dev.master, b"\xF7")
        self.start(cz)
        rc, out = run(["send", p, "--dev", self.name, "--baud", "0"])
        self.assertEqual(rc, 0, out)
        self.assertEqual(seen, ["head", "body"])
        self.assertEqual(bytes(self.dev.rx), tone)

    def test_cz_send_no_answer(self):
        tone = bytes([0xF0, 0x44, 0, 0, 0x70, 0x20, 0x21]) + bytes(256) + b"\xF7"
        p = self.file("cz.syx", tone)
        self.start()
        rc, out = run(["send", p, "--dev", self.name, "--baud", "0", "--timeout", "0.3"])
        self.assertEqual(rc, 1)
        self.assertIn("did not answer", out)
        self.assertEqual(bytes(self.dev.rx), tone[:7])   # the data is not sent unasked

    def test_cz_dump(self):
        data = bytes((k * 3) & 0x0F for k in range(256))
        state = []

        def cz(dev, got):
            if dev.rx == b"\xF0\x44\x00\x00\x72\x10\x22" and not state:
                state.append(1)
                os.write(dev.master, b"\xF0\x44\x00\x00\x72\x30")
            elif dev.rx.endswith(b"\x72\x31") and len(state) == 1:
                state.append(2)
                os.write(dev.master, data[:100] + b"\xFE" + data[100:] + b"\xF7")
            elif dev.rx.endswith(b"\xF7") and len(state) == 2:
                state.append(3)
        self.start(cz)
        out_path = os.path.join(self.dir, "cz_out.syx")
        rc, out = run(["dump", out_path, "--cz", "22", "--channel", "3", "--dev", self.name, "--baud", "0"])
        time.sleep(0.1)
        self.assertEqual(rc, 0, out)
        self.assertEqual(state, [1, 2, 3])
        self.assertEqual(read(out_path), bytes([0xF0, 0x44, 0, 0, 0x72, 0x20, 0x22]) + data + b"\xF7")

    def test_emu_closed_loop_nak_then_ack(self):
        head = bytes([0xF0, 0x18, 0x0F, 0x00, 0x55, 0x10, 0x01]) + bytes(10) + b"\xF7"
        pkt = bytes([0xF0, 0x18, 0x0F, 0x00, 0x55, 0x10, 0x02, 0x01, 0x00]) + bytes(40) + b"\xF7"
        p = self.file("p2k.syx", head + pkt)
        count = {"pkt": 0}

        def emu(dev, got):
            if dev.rx.endswith(b"\xF7"):
                if dev.rx.endswith(pkt) and count["pkt"] == 0:
                    count["pkt"] = 1
                    os.write(dev.master, b"\xF0\x18\x0F\x00\x55\x7E\x01\x00\xF7")   # NAK
                else:
                    os.write(dev.master, b"\xF0\x18\x0F\x00\x55\x7F\x00\x00\xF7")   # ACK
        self.start(emu)
        rc, out = run(["send", p, "--dev", self.name, "--baud", "0"])
        self.assertEqual(rc, 0, out)
        self.assertIn("NAK", out)
        self.assertEqual(bytes(self.dev.rx), head + pkt + pkt)

    def test_emu_closed_loop_dump(self):
        head = bytes([0xF0, 0x18, 0x0F, 0x00, 0x55, 0x10, 0x01]) + bytes(10) + b"\xF7"
        pkts = [bytes([0xF0, 0x18, 0x0F, 0x00, 0x55, 0x10, 0x02, k, 0x00]) + bytes(20) + b"\xF7" for k in (1, 2)]
        req = "f0180f005511020000f7"
        acks = []

        def emu(dev, got):
            if dev.rx.endswith(bytes.fromhex(req)):
                os.write(dev.master, head)
            elif dev.rx.endswith(b"\x55\x7F\x00\x00\xF7") and not acks:
                acks.append(0)
                os.write(dev.master, b"\xF0\x18\x0F\x00\x55\x7C\xF7" + pkts[0])
            elif dev.rx.endswith(b"\x55\x7F\x01\x00\xF7") and acks == [0]:
                acks.append(1)
                os.write(dev.master, pkts[1])
            elif dev.rx.endswith(b"\x55\x7F\x02\x00\xF7") and acks == [0, 1]:
                acks.append(2)
        self.start(emu)
        out_path = os.path.join(self.dir, "p2k.syx")
        rc, out = run(["dump", out_path, req, "--dev", self.name, "--baud", "0", "--idle", "300"])
        time.sleep(0.1)
        self.assertEqual(rc, 0, out)
        self.assertEqual(acks, [0, 1, 2])
        self.assertEqual(read(out_path), head + b"".join(pkts))

    def test_dump_request(self):
        reply = [roland_dt1(0x10, 0x46, [0, 0, 0, 0], [k] * 64) for k in range(3)]
        req = "f041104611000000000000001070f7"

        def jv(dev, got):
            if dev.rx.endswith(bytes.fromhex(req)):
                for r in reply:
                    os.write(dev.master, r + b"\xFE")
                    time.sleep(0.05)
        self.start(jv)
        out_path = os.path.join(self.dir, "jv.syx")
        rc, out = run(["dump", out_path, req, "--dev", self.name, "--baud", "0", "--idle", "300"])
        self.assertEqual(rc, 0, out)
        self.assertEqual(read(out_path), b"".join(reply))


if __name__ == "__main__":
    unittest.main(verbosity=1)
