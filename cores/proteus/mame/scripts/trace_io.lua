-- Log CPU writes to the Proteus sound chip and other I/O to a text file.
-- Usage: proteus proteusxr -seconds_to_run 10 -autoboot_script mame/scripts/trace_io.lua
-- Env: PROTEUS_TRACE (output path, default build/trace_io.txt)
local out_path = os.getenv("PROTEUS_TRACE") or "build/trace_io.txt"
local f = assert(io.open(out_path, "w"))
local space = manager.machine.devices[":maincpu"].spaces["program"]
local taps = {}

local function now() return manager.machine.time:as_double() end

local function tap(name, lo, hi, kind)
  local fn = function(offset, data, mask)
    f:write(string.format("%.6f %s %s %06x %04x %04x\n", now(), name, kind, offset, data, mask))
    return nil
  end
  if kind == "W" then
    taps[#taps + 1] = space:install_write_tap(lo, hi, name .. "_w", fn)
  else
    taps[#taps + 1] = space:install_read_tap(lo, hi, name .. "_r", fn)
  end
end

tap("snd", 0x800000, 0x8003ff, "W")
tap("snd", 0x800000, 0x8003ff, "R")
tap("lat4", 0x400000, 0x400001, "W")
tap("latb", 0xb00000, 0xb00001, "W")
tap("c00", 0xc00000, 0xc00001, "R")
tap("d00", 0xd00000, 0xd00003, "W")
tap("d00", 0xd00000, 0xd00003, "R")
tap("a00", 0xa00000, 0xa00001, "R")
tap("mfp", 0x900000, 0x90002f, "W")
tap("pan", 0x500000, 0x50004f, "W")
tap("udr", 0x90002e, 0x90002f, "R")

-- Run length is set on the command line with -seconds_to_run.
_G.proteus_trace_stop = emu.add_machine_stop_notifier(function() f:close() end)
_G.proteus_trace_taps = taps
