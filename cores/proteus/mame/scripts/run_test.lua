-- Combined start-up for automated runs: run the listed scripts (default: trace I/O).
-- Env: PROTEUS_SCRIPTS (colon-separated list, default "trace_io.lua")
local dir = (debug.getinfo(1, "S").source:sub(2)):match("(.*/)") or "./"
for name in string.gmatch(os.getenv("PROTEUS_SCRIPTS") or "trace_io.lua", "[^:]+") do
  dofile(dir .. name)
end
