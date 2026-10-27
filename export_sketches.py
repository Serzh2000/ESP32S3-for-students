"""Раскладывает примеры code/*.cpp по папкам для Arduino IDE.

Запуск:  python export_sketches.py
Результат: sketches/<имя>/<имя>.ino — каждую папку можно открыть в Arduino IDE.
Фрагменты (файлы без функции setup()) пропускаются: их вставляют в готовый скетч.
"""
import re
import shutil
from pathlib import Path

ROOT = Path(__file__).parent
OUT = ROOT / 'sketches'


def is_full_sketch(src: str) -> bool:
    return re.search(r'^void\s+setup\s*\(', src, flags=re.M) is not None


def main():
    if OUT.exists():
        shutil.rmtree(OUT)
    done, skipped = 0, []
    for cpp in sorted((ROOT / 'code').glob('*.cpp')):
        src = cpp.read_text(encoding='utf-8')
        if not is_full_sketch(src):
            skipped.append(cpp.stem)
            continue
        folder = OUT / cpp.stem
        folder.mkdir(parents=True)
        (folder / f'{cpp.stem}.ino').write_text(src, encoding='utf-8')
        done += 1
    print(f'скетчей: {done}; фрагменты пропущены: {", ".join(skipped) or "нет"}')


if __name__ == '__main__':
    main()
