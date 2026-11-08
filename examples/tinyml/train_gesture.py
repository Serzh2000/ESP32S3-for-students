"""Опыт 2 урока 42: классификатор жестов по окнам акселерометра.

CSV с платы:  session,label,mx,my,mz,sx,sy,sz   (среднее и разброс по осям, в g)
Запуск:
    python train_gesture.py --demo              # синтетические данные для проверки цепочки
    python train_gesture.py --csv gestures.csv  # ваши измерения
Результат: матрица ошибок на отложенных сеансах (stderr) и блок C++ с моделью (stdout).
"""
import argparse
import sys
import numpy as np
import tinyml_nn as nn

NAMES = ["покой", "тряска", "наклон"]
NF = 6


def demo_data():
    """Три класса, по три сеанса; у каждого сеанса свой небольшой сдвиг датчика."""
    rng = np.random.default_rng(3)
    rows = []
    for session in range(9):
        label = session % 3
        shift = rng.normal(0, 0.03, 3)                  # плата лежит чуть иначе
        for _ in range(60):
            if label == 0:
                m, s = [0.0, 0.0, 1.0], [0.01, 0.01, 0.01]
            elif label == 1:
                m, s = [0.0, 0.0, 1.0], [0.45, 0.45, 0.35]
            else:
                m, s = [0.0, 0.7, 0.7], [0.03, 0.03, 0.03]
            mean = np.array(m) + shift + rng.normal(0, 0.02, 3)
            std = np.abs(np.array(s) * rng.uniform(0.7, 1.3, 3))
            rows.append([session, label, *mean, *std])
    return np.array(rows)


def confusion(truth, pred, k):
    m = np.zeros((k, k), int)
    for t, p in zip(truth, pred):
        m[t, p] += 1
    return m


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--csv")
    ap.add_argument("--demo", action="store_true")
    a = ap.parse_args()
    if a.demo:
        data = demo_data()
    elif a.csv:
        data = np.loadtxt(a.csv, delimiter=",", comments="#")
    else:
        sys.exit("Укажите --demo или --csv файл")

    session, label, feats = data[:, 0].astype(int), data[:, 1].astype(int), data[:, 2:]
    k = int(label.max()) + 1
    # Тест = целый последний сеанс каждого класса: соседние окна не попадают в обе части
    test_sessions = {max(session[label == c]) for c in range(k)}
    is_test = np.isin(session, list(test_sessions))
    mean, std = feats[~is_test].mean(axis=0), feats[~is_test].std(axis=0) + 1e-6
    x = (feats - mean) / std

    sizes = [NF, 8, k]
    ws, bs = nn.train(x[~is_test], label[~is_test], sizes, softmax=True, epochs=2000, lr=0.02)

    for title, w in (("float32", ws), ("int8", nn.dequantized(ws))):
        pred = nn.forward(w, bs, x[is_test], True)[-1].argmax(axis=1)
        acc = (pred == label[is_test]).mean()
        print(f"\nТест ({title}), точность {acc:.1%}, окон: {is_test.sum()}", file=sys.stderr)
        print(confusion(label[is_test], pred, k), file=sys.stderr)

    print("// классы:", ", ".join(f"{i}={NAMES[i] if i < len(NAMES) else i}" for i in range(k)))
    print(f"const int NC = {k};")
    print(nn.export_cpp(ws, bs, sizes))
    for name, arr in (("FMEAN", mean), ("FSTD", std)):
        print(f"const float {name}[{NF}] = {{ " + ", ".join(f"{v:.5f}f" for v in arr) + " };")


if __name__ == "__main__":
    main()
