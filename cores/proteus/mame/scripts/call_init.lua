-- Call the firmware's diagnostics "Initialize" routine once, as a subroutine,
-- so preset RAM gets its factory contents. The NVRAM file then keeps them.
-- Env: PROTEUS_INIT_ENTRY (hex address; v2.30 ProtoLogic = 7b3c)
--      PROTEUS_INIT_AT (emulated seconds to wait before calling; default 2)
local entry = tonumber(os.getenv("PROTEUS_INIT_ENTRY") or "7b3c", 16)
local at = tonumber(os.getenv("PROTEUS_INIT_AT") or "2")
local cpu = manager.machine.devices[":maincpu"]
local space = cpu.spaces["program"]
local done = false

_G.proteus_init_sub = emu.add_machine_frame_notifier(function()
  if done or manager.machine.time:as_double() < at then return end
  local sr = cpu.state["SR"].value
  if (sr & 0x0700) ~= 0 then return end -- wait until not inside an interrupt handler
  done = true
  local pc = cpu.state["PC"].value
  local sp = cpu.state["SP"].value - 4
  space:write_u32(sp, pc)
  cpu.state["SP"].value = sp
  cpu.state["PC"].value = entry
  emu.print_info(string.format("call_init: called %06x from %06x", entry, pc))
end)
