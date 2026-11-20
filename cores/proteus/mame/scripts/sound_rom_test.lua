-- Call the firmware's Sound ROM test (v2.30: 0x2bd2, arg word = bank 0/1) as a subroutine
-- and log its result (d0 = 0 pass, -1 fail) with a debugger breakpoint on its rts.
-- Needs -debug -debugger none -log. Env: PROTEUS_ROMTEST_BANK (0 or 1), PROTEUS_ROMTEST_AT (s)
local entry, ret_bp = 0x2bd2, 0x2cbe
local bank = tonumber(os.getenv("PROTEUS_ROMTEST_BANK") or "0")
local at = tonumber(os.getenv("PROTEUS_ROMTEST_AT") or "3")
local cpu = manager.machine.devices[":maincpu"]
local space = cpu.spaces["program"]
local done = false
_G.proteus_romtest = emu.add_machine_frame_notifier(function()
  if done or manager.machine.time:as_double() < at then return end
  if (cpu.state["SR"].value & 0x0700) ~= 0 then return end
  done = true
  cpu.debug:bpset(ret_bp, "1", 'logerror "ROMTEST d0=%x\\n",d0; go')
  for _, a in ipairs({0x2bd2, 0x2c44, 0x2c7e, 0x2cb4, 0x2ca2, 0x2d30}) do cpu.debug:bpset(a, "1", string.format('logerror "RT %x d0=%%x d1=%%x d2=%%x\\n",d0,d1,d2; go', a)) end
  manager.machine.debugger.execution_state = "run"
  local sp = cpu.state["SP"].value - 2
  space:write_u16(sp, bank)
  sp = sp - 4
  space:write_u32(sp, cpu.state["PC"].value)
  cpu.state["SP"].value = sp
  cpu.state["PC"].value = entry
  emu.print_info(string.format("sound_rom_test: bank %d", bank))
end)
