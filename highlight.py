"""Подсветка примеров кода курса (chapters/*.tex).

Запуск:  python highlight.py
  1) если в chapters/*.tex есть блоки \\begin{srccode}{имя} ... \\end{srccode},
     они выносятся в code/имя.cpp и заменяются на \\codeinput{имя};
  2) для каждого code/*.cpp генерируется code/*.tex (Pygments -> LaTeX)
     и code/pygments-style.tex с макросами \\PY.
Дальше правьте .cpp и снова запускайте скрипт. Нужны только Python + Pygments.
"""
import re
from pathlib import Path
from pygments import highlight
from pygments.lexers import CppLexer
from pygments.formatters import LatexFormatter

ROOT = Path(__file__).parent
CHAPTERS = ROOT / 'chapters'
CODE = ROOT / 'code'
CODE.mkdir(exist_ok=True)

def extract(m):
    name, body = m.group(1), m.group(2)
    (CODE / f'{name}.cpp').write_text(body, encoding='utf-8')
    return f'\\codeinput{{{name}}}'

for tex in sorted(CHAPTERS.glob('*.tex')):
    src = tex.read_text(encoding='utf-8')
    src, n = re.subn(r'\\begin\{srccode\}\{([\w-]+)\}\n(.*?)\\end\{srccode\}', extract, src, flags=re.S)
    if n:
        tex.write_text(src, encoding='utf-8')
        print(f'{tex.name}: extracted {n} blocks')

fmt = LatexFormatter(style='friendly', commandprefix='PY')
(CODE / 'pygments-style.tex').write_text(fmt.get_style_defs(), encoding='utf-8')

files = sorted(CODE.glob('*.cpp'))
for cpp in files:
    body = highlight(cpp.read_text(encoding='utf-8'), CppLexer(), fmt)
    body = re.sub(r'^\\begin\{Verbatim\}\[.*?\]\n', '', body)
    body = body.replace('\\end{Verbatim}\n', '')
    (CODE / f'{cpp.stem}.tex').write_text(
        '\\begin{CodeVerbatim}\n' + body + '\\end{CodeVerbatim}\n', encoding='utf-8')
print('highlighted', len(files), 'files')
