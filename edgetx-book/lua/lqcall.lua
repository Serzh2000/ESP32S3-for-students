-- SCRIPTS/FUNCTIONS/lqcall.lua : функциональный скрипт
-- Special Functions -> Lua script -> lqcall. Пока условие активно,
-- каждые 10 секунд проговаривает качество связи (RQly).

local PERIOD = 1000     -- в «тиках» getTime(): 1 тик = 10 мс, 1000 = 10 с
local nextTime = 0

local function init()
  nextTime = 0
end

local function run()
  local now = getTime()
  if now >= nextTime then
    local lq = getValue("RQly")
    playNumber(lq, UNIT_PERCENT, 0)
    nextTime = now + PERIOD
  end
end

return { init = init, run = run }
