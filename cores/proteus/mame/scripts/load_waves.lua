-- Load a sound ROM image into the ":snd" region at start-up, replacing what MAME loaded
-- (the driver loads the dumped Proteus/1 ROMs; use this to compare with the rebuilt image).
-- Env: PROTEUS_WAVES  path to a big-endian 16-bit image (default roms/_rebuilt/proteus1_waves.bin)
--      PROTEUS_WAVES_OFFSET  byte offset in the region (default 0)
local path = os.getenv("PROTEUS_WAVES") or "roms/_rebuilt/proteus1_waves.bin"
local base = tonumber(os.getenv("PROTEUS_WAVES_OFFSET") or "0")
local region = manager.machine.memory.regions[":snd"]
local f = assert(io.open(path, "rb"))
local data = f:read("a")
f:close()
for i = 1, #data - 1, 2 do
  local hi, lo = data:byte(i, i + 1)
  region:write_u16(base + i - 1, (hi << 8) | lo)
end
assert(region:read_u16(base) == ((data:byte(1) << 8) | data:byte(2)), "region byte order mismatch")
emu.print_info(string.format("load_waves: %d bytes from %s at 0x%x", #data, path, base))
