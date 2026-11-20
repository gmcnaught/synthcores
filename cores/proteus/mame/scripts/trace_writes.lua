-- Log every CPU write ("<seconds> <addr> <data> <mask>") to PROTEUS_TRACE for the first
-- PROTEUS_TRACE_SECS emulated seconds, then exit MAME (-seconds_to_run takes whole seconds only).
-- For comparison with the HDL system simulation (sim/sys_main.cpp).
local f = assert(io.open(os.getenv("PROTEUS_TRACE") or "build/mame_writes.txt", "w"))
local limit = tonumber(os.getenv("PROTEUS_TRACE_SECS") or "0.5")
local space = manager.machine.devices[":maincpu"].spaces["program"]
local done = false
_G.proteus_wtap = space:install_write_tap(0x000000, 0xffffff, "allw", function(offset, data, mask)
  if done then return end
  local t = manager.machine.time:as_double()
  if t > limit then
    done = true
    f:close()
    manager.machine:exit()
    return
  end
  f:write(string.format("%.6f %06x %04x %04x\n", t, offset, data, mask))
end)
