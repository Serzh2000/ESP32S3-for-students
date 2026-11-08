# Обучение нейросетей на NumPy (урок 42)

```
pip install numpy
python train_sine.py > sine_model.txt              # опыт 1: сеть 1-8-8-1 повторяет sin(x)
python train_gesture.py --demo > gesture_model.txt # опыт 2 на искусственных данных
python train_gesture.py --csv gestures.csv > gesture_model.txt   # на ваших данных
```

Качество (ошибка, матрица ошибок) печатается в stderr, блок C++ с весами int8 — в stdout.
Блок вставляют между строками `НАЧАЛО/КОНЕЦ СГЕНЕРИРОВАННОЙ МОДЕЛИ` в `code/l42_sine_nn.cpp`
или `code/l42_imu_infer.cpp`. CSV собирает скетч `code/l42_imu_collect.cpp`.
Сеть из `tinyml_nn.py` — учебная: полносвязные слои, ReLU, int8-квантование только весов.
Для промышленных моделей используйте TensorFlow Lite Micro или ESP-DL.
