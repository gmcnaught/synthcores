# SysEx dumps: `syx.py`

`syx.py` loads patch and preset dumps (`.syx`, or the SysEx in a `.mid`) into a core, and saves
dumps from it, through the core's MIDI UART (`/dev/ttyS1` on the MiSTer). It needs only Python 3
(the MiSTer has 3.9). The cores run their instruments' own firmware, so the firmware receives
and stores the data; the core's save file keeps it (JV-880 NVRAM, CZ-101 battery RAM, Proteus/1
`.sav`, P2K flash), written when the OSD opens (autosave) or with the OSD save item.

```sh
syx.py info bank.syx                        # what is in it: maker, model, command, checksums
syx.py send bank.syx --stop-midilink        # on the MiSTer, with the core loaded
syx.py dump out.syx F0180F005511040000F7    # send a request, save what comes back
syx.py dump tone.syx --cz 20                # Casio CZ: internal tone 1 over the CZ handshake
```

From the build machine, `modules/synthframe/bin/synth-mister` copies the tool and the files over:
`synth-mister syx-send bank.syx [-- OPTIONS]`, `synth-mister syx-dump OUT.syx REQUEST_HEX...`
(run `synth-mister midi-off` after loading the core). MidiLink holds the UART: `send` refuses
while it runs, and `--stop-midilink` stops it and the `uartmode` loop that restarts it. The next
core load starts MidiLink again.

Instead of this tool, any SysEx librarian on a computer can send a dump through a USB MIDI
interface and MidiLink; pace it as in the table below.

## How each message is sent

| Receiver | How | Default |
|---|---|---|
| Roland (`F0 41 ...`), JV-880 | message, then a gap | 40 ms (Roland: ≥ 20 ms between DT1 packets); DT1 checksums are checked first, a wrong one stops the send |
| E-mu Proteus/1 (`F0 18 04 ...`) | message, then a gap | 60 ms |
| E-mu P2K family open loop (`F0 18 0F dd 55 10 03/04`) | message, then a gap | 60 ms |
| E-mu P2K family closed loop (`10 01/02`) | waits for the ACK `55 7F pp pp` (through WAITs `55 7C`); NAK sends again, CANCEL stops | |
| Casio CZ tone (`F0 44 00 00 7n 20 pp` + 256 + `F7`) | sends the 7-byte header, waits for `F0 44 00 00 7n 30`, sends the data, waits for `F7` | |
| others | message, then a gap | 60 ms (Yamaha 100 ms) |

The gap counts from when the UART has sent the last byte (`tcdrain`). `--gap MS` overrides it.
Replies from the core are printed.

## Checked on the MiSTer (2026-11-26)

Each core: a dump made from the core (or built from its documented format), changed, sent back,
read back, then saved, the core reloaded and read back again.

| Core | Sent | Result |
|---|---|---|
| JV-880 | 80 DT1 (names of the 16 internal performances and 64 internal patches), 40 ms gap | all 80 shown on the LCD (I01–I64, perf I-01), kept after autosave and reload |
| CZ-101 | 16 tones into internal 1–16 over the handshake | read back byte for byte, kept after reload |
| Proteus/1 XR | 64 user presets (64–127, 265 bytes each), 11.4 s | read back byte for byte; program change 64 shows the new name; kept after reload |
| P2K | user presets 0–15, open loop (128 messages, 25.7 KB, 18.4 s); preset 1 closed loop | names read back; kept after reload; closed-loop dump (`11 02`) also works |

Notes from those runs:

- **JV-880:** RQ1 is answered for the system area (`00 00 00 00`) only; requests for patch and
  performance areas (`01 40 20 00`, `00 08 20 00`, ...) get no reply on the core. Loading (DT1) is
  not affected. Not yet compared with the reference.
- **P2K:** the OS forces the 4th character of a preset name to `:` (category prefix); after each
  closed-loop packet it sends WAIT, then the ACK.
- **CZ-101 MIDI out:** 3 bytes in about 12,700 read back with bit 7 set (`00` read as `80`), so a
  dump from the CZ may need repeating; what was sent to it was stored correctly each time.
- **Proteus/1:** the factory RAM preset dumps on sysexdb.com need a login to download; without
  one the site returns an HTML page, which `syx.py info` reports.

Unit tests (a fake device on a pty, no MiSTer): `python3 modules/synthframe/tests/syx/test_syx.py`.
