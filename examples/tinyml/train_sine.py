"""Опыт 1 урока 42: учим сеть 1-8-8-1 повторять sin(x) на отрезке [0, 2π]."""
import sys
import numpy as np
import tinyml_nn as nn

sizes = [1, 8, 8, 1]
rng = np.random.default_rng(7)
x = rng.uniform(0, 2 * np.pi, (2000, 1))
y = np.sin(x) + rng.normal(0, 0.02, x.shape)           # лёгкий шум, как в настоящих измерениях
xn = x / (2 * np.pi)                                    # нормализация входа в 0..1

ws, bs = nn.train(xn, y, sizes, softmax=False, epochs=6000, lr=0.01)

xt = np.linspace(0, 2 * np.pi, 400).reshape(-1, 1)      # независимая проверка
truth = np.sin(xt)
f32 = nn.forward(ws, bs, xt / (2 * np.pi), False)[-1]
q8 = nn.forward(nn.dequantized(ws), bs, xt / (2 * np.pi), False)[-1]
print(f"макс. ошибка float32: {np.abs(f32 - truth).max():.3f}", file=sys.stderr)
print(f"макс. ошибка int8:    {np.abs(q8 - truth).max():.3f}", file=sys.stderr)
print(f"средняя ошибка int8:  {np.abs(q8 - truth).mean():.3f}", file=sys.stderr)
print(nn.export_cpp(ws, bs, sizes))
