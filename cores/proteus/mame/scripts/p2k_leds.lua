-- proteus2k: log every change of the panel LED latches (0x900000 low byte, 0xA00000 high byte)
-- to P2K_LEDLOG as "<seconds> <latch> <byte> word=<16-bit word> pc=<PC>". P2K_ADC="t:ch:raw,..."
-- makes knob ADC channel ch read raw (0-255) from t seconds on (pots 1-4 are channels 1, 0, 4, 3).
-- Used with P2K_KEYS (scripted presses) for the LED map in cores/proteus/docs/p2k-findings.md:
--   P2K_KEYS=keys.txt P2K_LEDLOG=leds.txt tools/p2k-run.sh 400 OUT -seconds_to_run 43 \
--     -autoboot_script cores/proteus/mame/scripts/p2k_leds.lua
local f = assert(io.open(os.getenv("P2K_LEDLOG") or "leds.txt", "w"))
local cpu = manager.machine.devices[":maincpu"]
local space = cpu.spaces["program"]
local cur = {}
_G.p2k_ledtap = space:install_write_tap(0x900000, 0xa00003, "leds", function(offset, data, mask)
  for i = 0, 3 do
    local sh = (3 - i) * 8
    local a = (offset & 0xfffffffc) + i
    if ((mask >> sh) & 0xff) ~= 0 and (a == 0x900000 or a == 0xa00000) then
      local v = (data >> sh) & 0xff
      if cur[a] ~= v then
        cur[a] = v
        f:write(string.format("%.4f %06x %02x  word=%04x pc=%08x\n", manager.machine.time:as_double(), a, v,
          ((cur[0xa00000] or 0) << 8) | (cur[0x900000] or 0), cpu.state["PC"].value))
        f:flush()
      end
    end
  end
end)

local sched = {}
for t, ch, raw in (os.getenv("P2K_ADC") or ""):gmatch("([%d.]+):(%d+):(%d+)") do
  sched[#sched + 1] = { tonumber(t), tonumber(ch), tonumber(raw) }
end
if #sched > 0 then
  local sel = 0
  _G.p2k_adcw = space:install_write_tap(0xc00000, 0xc00003, "adcw", function(offset, data, mask)
    if (mask & 0xff000000) ~= 0 then sel = (data >> 24) & 7 end
  end)
  _G.p2k_adcr = space:install_read_tap(0xc00000, 0xc00003, "adcr", function(offset, data, mask)
    if (mask & 0xff000000) == 0 then return end
    local t, v = manager.machine.time:as_double(), nil
    for _, e in ipairs(sched) do if t >= e[1] and e[2] == sel then v = e[3] end end
    if v then return (data & 0x00ffffff) | (v << 24) end
  end)
end
