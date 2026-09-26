-- SCRIPTS/TELEMETRY/batt.lua : экран телеметрии для дисплея 128x64
-- Назначение: MDL -> Telemetry -> Screen 1 -> Script -> batt

local CELLS   = 4       -- число банок аккумулятора модели
local V_EMPTY = 3.3     -- напряжение пустой банки
local V_FULL  = 4.2     -- напряжение полной банки

local function drawBar(x, y, w, h, pct)
  lcd.drawRectangle(x, y, w, h)
  local fill = math.floor((w - 2) * pct / 100)
  if fill > 0 then
    lcd.drawFilledRectangle(x + 1, y + 1, fill, h - 2)
  end
end

local function run(event)
  local vbat = getValue("RxBt")          -- 0, если датчика нет
  local lq   = getValue("RQly")
  local rssi = getValue("1RSS")
  local cell = vbat / CELLS
  local pct  = (cell - V_EMPTY) / (V_FULL - V_EMPTY) * 100
  pct = math.max(0, math.min(100, pct))

  lcd.clear()
  lcd.drawText(0, 0, model.getInfo().name, INVERS)

  -- крупно: напряжение на банку
  lcd.drawNumber(0, 12, math.floor(cell * 100), DBLSIZE + PREC2)
  lcd.drawText(lcd.getLastRightPos() + 2, 20, "V/cell")
  drawBar(0, 32, 80, 10, pct)

  -- справа: связь
  lcd.drawText(86, 12, "LQ", SMLSIZE)
  lcd.drawNumber(100, 12, lq, (lq < 70) and BLINK or 0)
  lcd.drawText(86, 24, "RSSI", SMLSIZE)
  lcd.drawNumber(106, 24, rssi, 0)

  -- внизу: общее напряжение и таймер
  lcd.drawText(0, 48, "Pack")
  lcd.drawNumber(26, 48, math.floor(vbat * 10), PREC1)
  lcd.drawTimer(86, 48, model.getTimer(0).value)
  return 0
end

return { run = run }
