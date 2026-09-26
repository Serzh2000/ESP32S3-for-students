-- SCRIPTS/MIXES/thrlim.lua : микшерный скрипт «ограничитель газа»
-- Подключение: MDL -> Custom Scripts -> thrlim, вход Thr = стик газа.
-- Выход скрипта (источник "Lim") используйте в миксере канала газа.

local input = {
  { "Thr",   SOURCE },              -- источник: стик газа
  { "Limit", VALUE, 10, 100, 60 },  -- предел, %: от 10 до 100, по умолчанию 60
}

local output = { "Lim" }

local function run(thr, limit)
  -- thr приходит в диапазоне -1024..1024
  local pos = (thr + 1024) / 2048          -- 0..1
  local out = pos * limit / 100            -- 0..limit/100
  return math.floor(out * 2048 - 1024)     -- обратно в -1024..1024
end

return { input = input, output = output, run = run }
