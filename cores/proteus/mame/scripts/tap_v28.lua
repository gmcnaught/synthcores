local f = assert(io.open(os.getenv("OUT"), "w"))
local sp = manager.machine.devices[":maincpu"].spaces["program"]
local n = 0
_G.t28 = sp:install_read_tap(0x800380, 0x80039f, "v28", function(o, d, m)
  if n < 40 then n = n + 1; f:write(string.format("R +%02x %04x\n", o - 0x800380, d)) end
end)
_G.t28w = sp:install_write_tap(0x800380, 0x80039f, "v28w", function(o, d, m)
  if n < 40 then f:write(string.format("W +%02x %04x\n", o - 0x800380, d)) end
end)
_G.s28 = emu.add_machine_stop_notifier(function() f:close() end)
