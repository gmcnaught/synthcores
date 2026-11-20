-- Combined start-up for automated runs: load the rebuilt base ROM, then trace I/O.
-- Env: PROTEUS_SCRIPTS (colon-separated list, default "load_waves.lua:trace_io.lua")
local dir = (debug.getinfo(1, "S").source:sub(2)):match("(.*/)") or "./"
for name in string.gmatch(os.getenv("PROTEUS_SCRIPTS") or "load_waves.lua:trace_io.lua", "[^:]+") do
  dofile(dir .. name)
end
