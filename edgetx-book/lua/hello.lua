-- SCRIPTS/TOOLS/hello.lua : простейший скрипт-утилита
-- Запуск: SYS -> Tools -> hello. Выход: RTN.

local counter = 0

local function init()
  counter = 0
end

local function run(event)
  if event == EVT_VIRTUAL_EXIT then
    return 1                         -- ненулевое значение = завершить скрипт
  end
  if event == EVT_VIRTUAL_ENTER then
    counter = counter + 1
  end

  lcd.clear()
  lcd.drawText(0, 0, "Hello, EdgeTX!", INVERS)
  lcd.drawText(0, 16, "Model: " .. model.getInfo().name)
  lcd.drawText(0, 28, "ENTER pressed:")
  lcd.drawNumber(90, 28, counter, 0)
  lcd.drawText(0, 44, "Tx batt:")
  lcd.drawNumber(60, 44, math.floor(getValue("tx-voltage") * 10), PREC1)
  lcd.drawText(lcd.getLastRightPos(), 44, "V")
  return 0                           -- 0 = продолжать работу
end

return { init = init, run = run }
