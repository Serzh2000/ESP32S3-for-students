"""Крошечная нейросеть на NumPy: обучение, квантование весов в int8, экспорт в C++ (урок 42).

Полносвязная сеть: слои ReLU, на выходе либо линейный слой (регрессия),
либо softmax (классификация). Никаких TensorFlow и PyTorch: весь путь виден целиком.
"""
import numpy as np


def init(sizes, rng):
    ws, bs = [], []
    for n_in, n_out in zip(sizes[:-1], sizes[1:]):
        ws.append(rng.normal(0, np.sqrt(2.0 / n_in), (n_in, n_out)))
        bs.append(np.zeros(n_out))
    return ws, bs


def forward(ws, bs, x, softmax):
    acts = [x]
    for i, (w, b) in enumerate(zip(ws, bs)):
        z = acts[-1] @ w + b
        if i < len(ws) - 1:
            z = np.maximum(z, 0)                      # ReLU
        acts.append(z)
    if softmax:
        e = np.exp(acts[-1] - acts[-1].max(axis=1, keepdims=True))
        acts[-1] = e / e.sum(axis=1, keepdims=True)
    return acts


def train(x, y, sizes, softmax, epochs=3000, lr=0.01, seed=1):
    """y: для регрессии (N, 1) числа; для классификации (N,) номера классов."""
    rng = np.random.default_rng(seed)
    ws, bs = init(sizes, rng)
    m = [np.zeros_like(p) for p in ws + bs]            # Adam
    v = [np.zeros_like(p) for p in ws + bs]
    onehot = np.eye(sizes[-1])[y] if softmax else y
    for t in range(1, epochs + 1):
        acts = forward(ws, bs, x, softmax)
        grad = (acts[-1] - onehot) / len(x)            # dL/dz для MSE (с точностью до 2) и для softmax+CE
        gws, gbs = [None] * len(ws), [None] * len(bs)
        for i in range(len(ws) - 1, -1, -1):
            gws[i] = acts[i].T @ grad
            gbs[i] = grad.sum(axis=0)
            if i:
                grad = (grad @ ws[i].T) * (acts[i] > 0)
        for j, (p, g) in enumerate(zip(ws + bs, gws + gbs)):
            m[j] = 0.9 * m[j] + 0.1 * g
            v[j] = 0.999 * v[j] + 0.001 * g * g
            p -= lr * (m[j] / (1 - 0.9 ** t)) / (np.sqrt(v[j] / (1 - 0.999 ** t)) + 1e-8)
    return ws, bs


def quantize(ws):
    """Симметричное int8-квантование каждого слоя: w ≈ q * scale."""
    out = []
    for w in ws:
        scale = float(np.abs(w).max()) / 127.0
        out.append((np.round(w / scale).astype(np.int8), scale))
    return out


def dequantized(ws):
    return [q.astype(np.float64) * s for q, s in quantize(ws)]


def _arr(name, a, ctype, fmt):
    flat = a.flatten()
    rows = [", ".join(fmt.format(v) for v in flat[i:i + 8]) for i in range(0, len(flat), 8)]
    return f"const {ctype} {name}[{len(flat)}] = {{\n  " + ",\n  ".join(rows) + "\n};\n"


def export_cpp(ws, bs, sizes):
    """Текст C++ с весами int8 (порядок [вход][выход]), масштабами и смещениями."""
    text = []
    for i, ((q, s), b) in enumerate(zip(quantize(ws), bs), 1):
        text.append(f"// слой {i}: {sizes[i-1]} -> {sizes[i]}\n")
        text.append(_arr(f"W{i}", q, "int8_t", "{:d}"))
        text.append(f"const float S{i} = {s:.8f}f;\n")
        text.append(_arr(f"BIAS{i}", b, "float", "{:.5f}f"))
    n = sum(q.size for q, _ in quantize(ws))
    text.append(f"// всего весов: {n} (int8: {n} байт; во float было бы {4 * n})\n")
    return "".join(text)
