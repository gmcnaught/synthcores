-- Sample the proteus2k CPU every 0.5 s of emulated time (research aid). If it sits in the boot
-- fatal handler (PC unchanged between samples), print the message it was given and the exception frame.
local cpu = manager.machine.devices[":maincpu"]
local mem = cpu.spaces["program"]
local last, done, prev = -1, false, -1
local function cstr(a)
    local s = ""
    for i = 0, 79 do local c = mem:read_u8(a + i); if c == 0 then break end; s = s .. string.char(c) end
    return s
end
emu.register_periodic(function()
    local t = manager.machine.time.seconds
    if t - last < 0.5 then return end
    last = t
    local pc = cpu.state["PC"].value
    print(string.format("t=%.2f PC=%08X SR=%04X", t, pc, cpu.state["SR"].value))
    local stuck = (pc == prev); prev = pc
    if stuck and not done then
        done = true
        local sp = cpu.state["SP"].value
        local a5 = cpu.state["A5"].value
        print(string.format("fatal: A5=%08X '%s' SP=%08X", a5, cstr(a5), sp))
        for i = 0, 15 do io.write(string.format("%08X ", mem:read_u32(sp + i * 4))) end
        print()
    end
end)
