// Шаг 1. Один слой нейронов: 2 входа, 3 нейрона, ReLU. Веса выбраны заранее, а не обучены.
#include <Arduino.h>

const int N_IN = 2, N_OUT = 3;
// вес от входа i к нейрону j лежит в W[i * N_OUT + j]
const float W[N_IN * N_OUT] = { 2, 2, -1,      // от входа 0 к нейронам 0, 1, 2
                                1, 1, -2 };    // от входа 1 к нейронам 0, 1, 2
const float B[N_OUT] = { 0.3f, -0.5f, 0.1f };  // смещения

void dense(const float* in, float* out) {
  for (int j = 0; j < N_OUT; ++j) {
    float sum = B[j];
    for (int i = 0; i < N_IN; ++i) sum += in[i] * W[i * N_OUT + j];
    out[j] = sum > 0 ? sum : 0;                 // ReLU
  }
}

void setup() {
  Serial.begin(115200);
  float in[N_IN] = { 0.5f, -1.0f }, out[N_OUT];
  dense(in, out);
  for (int j = 0; j < N_OUT; ++j) Serial.printf("neuron %d: %.2f\n", j, out[j]);
}

void loop() {}
