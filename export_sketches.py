"""Раскладывает примеры code/*.cpp по папкам: для Arduino IDE и для PlatformIO.

Запуск:  python export_sketches.py

Результат (папка sketches/ хранится в репозитории):
  sketches/arduino/<урок>/<имя>/<имя>.ino   каждую папку открывают в Arduino IDE
  sketches/platformio/platformio.ini         один проект PlatformIO, по окружению на скетч
  sketches/README.md                         указатель: урок, скетч, нужные библиотеки

Источник правды — code/*.cpp (из них же собирается книга). Папку sketches/ руками не правьте:
изменения затрутся при следующем запуске. Файлы без функции setup() — фрагменты, их вставляют
в готовый скетч по тексту урока; в sketches/ они не попадают.
"""
import re
import shutil
from pathlib import Path

ROOT = Path(__file__).parent
OUT = ROOT / 'sketches'

# Заголовок -> (библиотека для lib_deps PlatformIO, название для Arduino IDE)
LIBS = {
    'Adafruit_GFX.h': ('adafruit/Adafruit GFX Library @ ^1.11.9', 'Adafruit GFX Library'),
    'Adafruit_SSD1306.h': ('adafruit/Adafruit SSD1306 @ ^2.5.0', 'Adafruit SSD1306'),
    'Adafruit_NeoPixel.h': ('adafruit/Adafruit NeoPixel @ ^1.12.0', 'Adafruit NeoPixel'),
    'Adafruit_ST7789.h': ('adafruit/Adafruit ST7735 and ST7789 Library @ ^1.10.4',
                          'Adafruit ST7735 and ST7789 Library'),
    'ArduinoJson.h': ('bblanchon/ArduinoJson @ ^7.0.0', 'ArduinoJson 7'),
    'DHT.h': ('adafruit/DHT sensor library @ ^1.4.6', 'DHT sensor library (Adafruit)'),
    'DallasTemperature.h': ('milesburton/DallasTemperature @ ^3.9.1', 'DallasTemperature'),
    'OneWire.h': ('paulstoffregen/OneWire @ ^2.3.8', 'OneWire'),
    'PubSubClient.h': ('knolleary/PubSubClient @ ^2.8', 'PubSubClient'),
    'WiFiManager.h': ('tzapu/WiFiManager @ 2.0.17', 'WiFiManager 2.0.17'),
    # Версии ESPUI и его зависимостей — те же, что проверены в examples/web-panels
    'ESPUI.h': (None, 'ESPUI 2.2.4, ESP Async WebServer, Async TCP, ArduinoJson'),
}
ESPUI_DEPS = [
    'https://github.com/s00500/ESPUI.git#1eb5e2ffc4eebf3a854247fd5208a5b88edc051e',
    'https://github.com/ESP32Async/ESPAsyncWebServer.git#4d4eaca1833c3c11940a988fc8a6c0bdf734bbd2',
    'https://github.com/ESP32Async/AsyncTCP.git#9f03d11a3e7e0a2205c4d302446310f74f9e6380',
    'bblanchon/ArduinoJson @ ^7.0.0',
]

# Особые настройки отдельных скетчей (ключ — префикс имени скетча)
EXTRA_FLAGS = {
    # USB-OTG (TinyUSB) вместо порта UART/CDC: клавиатура, мышь, громкость, свой CDC
    'l19_': ['-DARDUINO_USB_MODE=0', '-DARDUINO_USB_CDC_ON_BOOT=0',
             '-I$PROJECT_PACKAGES_DIR/framework-arduinoespressif32/libraries/FS/src'],
    'l34_': ['-DARDUINO_USB_MODE=0', '-DARDUINO_USB_CDC_ON_BOOT=0'],
    # камера: плата с PSRAM (ESP32-S3-EYE и подобные); тип PSRAM сверьте со своим модулем
    'l20_': ['-DBOARD_HAS_PSRAM'],
}
EXTRA_OPTS = {
    'l19_': ['lib_ldf_mode = chain+'],
    'l34_': ['lib_ldf_mode = chain+'],
    'l20_': ['board_build.arduino.memory_type = qio_opi',
             'board_build.partitions = default_8MB.csv'],
}

NOTE = {
    'l19_': 'в Arduino IDE: Tools → USB Mode → USB-OTG (TinyUSB)',
    'l34_': 'в Arduino IDE: Tools → USB Mode → USB-OTG (TinyUSB)',
    'l20_': 'плата с камерой и PSRAM; Tools → PSRAM → по вашему модулю',
}

# Скетчи, которые не собираются как обычный Arduino-проект
NOT_ARDUINO = {
    'l36_main': 'ESP-IDF, см. examples/ulp_button',
    'l36_ulp': 'программа ULP RISC-V, см. examples/ulp_button',
}


# Скетчи, которых нет в тексте глав, но место которым известно
OVERRIDE = {'l8_webserver': '44-html'}


def is_full_sketch(src: str) -> bool:
    return re.search(r'^void\s+setup\s*\(', src, flags=re.M) is not None


def lesson_folders():
    """имя скетча -> папка урока (по первому \\codeinput в главе)."""
    result = {}
    for tex in sorted((ROOT / 'chapters').glob('[0-9][0-9]-*.tex')):
        for name in re.findall(r'\\codeinput(?:\[[^\]]*\])?\{([^}]+)\}', tex.read_text(encoding='utf-8')):
            result.setdefault(name, tex.stem)
    return result


def by_number(name: str) -> str:
    """Запасной вариант, если скетч не вставлен в главу: l5_pot -> 06-adc.
    Короткие префиксы l0_..l9_ — номера уроков до перестановки глав (4, 5, 6, 7, 8 -> 4, 6, 7, 8, 5)."""
    number = int(re.match(r'l(\d+)_', name).group(1))
    if len(re.match(r'l(\d+)_', name).group(1)) == 1:
        number = {5: 6, 6: 7, 7: 8, 8: 5}.get(number, number)
    for tex in sorted((ROOT / 'chapters').glob(f'{number:02d}-*.tex')):
        return tex.stem
    return 'other'


def prefix_of(name: str) -> str:
    return re.match(r'(l\d+_)', name).group(1)


def per_prefix(table, name):
    return table.get(prefix_of(name), [] if table is not NOTE else '')


def main():
    if OUT.exists():
        # кэш сборки PlatformIO (.pio) не трогаем: его пересборка занимает минуты
        shutil.rmtree(OUT / 'arduino', ignore_errors=True)
        shutil.rmtree(OUT / 'platformio' / 'src', ignore_errors=True)
    folders = lesson_folders()
    rows = []          # (папка урока, имя, библиотеки)
    fragments = []
    for cpp in sorted((ROOT / 'code').glob('*.cpp')):
        name = cpp.stem
        src = cpp.read_text(encoding='utf-8')
        if name in NOT_ARDUINO or not is_full_sketch(src):
            fragments.append(name)
            continue
        lesson = OVERRIDE.get(name) or folders.get(name) or by_number(name)
        target = OUT / 'arduino' / lesson / name
        target.mkdir(parents=True)
        (target / f'{name}.ino').write_text(src, encoding='utf-8')
        headers = re.findall(r'#include\s*[<"]([^>"]+)', src)
        cpp_dir = OUT / 'platformio' / 'src' / lesson
        cpp_dir.mkdir(parents=True, exist_ok=True)
        (cpp_dir / f'{name}.cpp').write_text('#include <Arduino.h>\n' + src, encoding='utf-8')
        rows.append((lesson, name, [h for h in headers if h in LIBS]))

    # --- PlatformIO: один проект, окружение на скетч, исходники берутся из ../arduino
    ini = [
        '; Все скетчи книги в одном проекте PlatformIO. Собрать и загрузить один скетч:',
        ';   pio run -e l1_blink -t upload',
        ';   pio device monitor',
        '; Список окружений: pio project config   (или просто откройте файл)',
        '; Исходники в src/<урок>/<скетч>.cpp — тот же текст, что в .ino для Arduino IDE (плюс #include <Arduino.h>).',
        '; Платформа pioarduino (ядро Arduino-ESP32 3.x). Актуальный адрес и версию сверьте на',
        '; https://github.com/pioarduino/platform-espressif32',
        '',
        '[platformio]',
        f'default_envs = {rows[0][1]}',
        '',
        '[env]',
        'platform = https://github.com/pioarduino/platform-espressif32/releases/download/stable/platform-espressif32.zip',
        'board = esp32-s3-devkitc-1',
        'framework = arduino',
        'monitor_speed = 115200',
        'monitor_filters = esp32_exception_decoder, time',
        'build_flags = -DCORE_DEBUG_LEVEL=1',
        '',
    ]
    for lesson, name, headers in rows:
        deps = []
        for h in headers:
            if h == 'ESPUI.h':
                deps += ESPUI_DEPS
            elif LIBS[h][0]:
                deps.append(LIBS[h][0])
        if 'DHT.h' in headers:
            deps.append('adafruit/Adafruit Unified Sensor @ ^1.1.14')
        deps = list(dict.fromkeys(deps))
        flags = per_prefix(EXTRA_FLAGS, name)
        opts = per_prefix(EXTRA_OPTS, name)
        ini.append(f'[env:{name}]')
        ini.append(f'build_src_filter = -<*> +<{lesson}/{name}.cpp>')
        if flags:
            ini.append('build_flags = ${env.build_flags} ' + ' '.join(flags))
        ini.extend(opts)
        if deps:
            ini.append('lib_deps =')
            ini.extend(f'    {d}' for d in deps)
        ini.append('')
    pio = OUT / 'platformio'
    pio.mkdir(parents=True, exist_ok=True)
    (pio / 'platformio.ini').write_text('\n'.join(ini), encoding='utf-8')
    (pio / '.gitignore').write_text('.pio/\n.vscode/\n', encoding='utf-8')

    # --- указатель
    md = [
        '# Скетчи к книге',
        '',
        'Папка создаётся командой `python export_sketches.py` из `code/*.cpp` — **руками не правьте**:',
        'правки пропадут при следующем запуске. Меняйте `code/*.cpp`, затем запустите скрипт.',
        '',
        '## Как пользоваться',
        '',
        '**Arduino IDE.** Откройте любую папку `arduino/<урок>/<скетч>/` (файл `.ino`). Плата — ESP32S3 Dev Module,',
        'ядро Arduino-ESP32 3.x. Нужные библиотеки перечислены в таблице ниже (Менеджер библиотек).',
        '',
        '**PlatformIO.** Откройте папку `platformio/` как проект и соберите нужный скетч по имени окружения:',
        '',
        '```',
        'pio run -e l1_blink -t upload',
        'pio device monitor',
        '```',
        '',
        'Библиотеки PlatformIO скачает сам (`lib_deps` в `platformio.ini`). Свой проект с библиотекой, тестами',
        'и отладкой — `../examples/platformio-lamp` (урок 41).',
        '',
        '## Что ещё лежит вне `sketches/`',
        '',
        '| Папка | Что это |',
        '|---|---|',
        '| `examples/platformio-lamp` | проект PlatformIO: модуль, тесты на компьютере, отладка (урок 41) |',
        '| `examples/tinyml` | обучение нейросетей на NumPy, экспорт весов (урок 42) |',
        '| `examples/debug_mean` | ESP-IDF и отладка по JTAG (урок 35) |',
        '| `examples/ulp_button` | программа ULP RISC-V, ESP-IDF (урок 36) |',
        '| `examples/web-panels` | точные версии библиотек для ESPUI и WiFiManager (уроки 39–40) |',
        '',
        '## Список скетчей',
        '',
        '| Урок | Скетч | Библиотеки (Arduino IDE) | Примечание |',
        '|---|---|---|---|',
    ]
    for lesson, name, headers in rows:
        libs = ', '.join(dict.fromkeys(LIBS[h][1] for h in headers)) or '—'
        md.append(f'| {lesson} | `{name}` | {libs} | {NOTE.get(prefix_of(name), "")} |')
    md += [
        '',
        '## Фрагменты',
        '',
        'Эти примеры — не готовые скетчи (нет `setup()` или это не Arduino-код); их вставляют в скетч по тексту урока:',
        '',
    ]
    for name in fragments:
        md.append(f'- `code/{name}.cpp`' + (f' — {NOT_ARDUINO[name]}' if name in NOT_ARDUINO else ''))
    (OUT / 'README.md').write_text('\n'.join(md) + '\n', encoding='utf-8')
    print(f'скетчей: {len(rows)}; фрагментов: {len(fragments)} ({", ".join(fragments) or "нет"})')


if __name__ == '__main__':
    main()
