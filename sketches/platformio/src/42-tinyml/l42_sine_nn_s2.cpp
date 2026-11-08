#include <Arduino.h>
// Шаг 2. Квантование: те же веса хранятся как int8 и один общий множитель scale.
#include <Arduino.h>

const float W[6] = { 2, 2, -1, 1, 1, -2 };      // исходные веса во float
const float B[3] = { 0.3f, -0.5f, 0.1f };
int8_t Q[6];                                     // те же веса в одном байте

float quantize(const float* w, int n, int8_t* q) {
  float maxAbs = 0;
  for (int i = 0; i < n; ++i) maxAbs = max(maxAbs, fabsf(w[i]));
  float scale = maxAbs / 127;
  for (int i = 0; i < n; ++i) q[i] = (int8_t)lroundf(w[i] / scale);
  return scale;
}

void dense(const float* in, int nIn, const int8_t* w, float scale,
           const float* b, float* out, int nOut, bool relu) {
  for (int j = 0; j < nOut; ++j) {
    float sum = 0;
    for (int i = 0; i < nIn; ++i) sum += in[i] * w[i * nOut + j];
    sum = sum * scale + b[j];                    // scale -- один раз на нейрон
    out[j] = (relu && sum < 0) ? 0 : sum;
  }
}

void setup() {
  Serial.begin(115200);
  float scale = quantize(W, 6, Q);
  Serial.printf("scale = %.5f, q =", scale);
  for (int i = 0; i < 6; ++i) Serial.printf(" %d", Q[i]);
  Serial.println();
  float in[2] = { 0.5f, -1.0f }, out[3];
  dense(in, 2, Q, scale, B, out, 3, true);       // ждём примерно 0.30, 0.00, 1.60
  for (int j = 0; j < 3; ++j) Serial.printf("neuron %d: %.3f\n", j, out[j]);
}

void loop() {}
