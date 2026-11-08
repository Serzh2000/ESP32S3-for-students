// Модульные тесты Debounce. На компьютере:  pio test -e native
// На плате (нужен подключённый ESP32-S3):    pio test -e esp32s3
#include <unity.h>
#include <Debounce.h>

void setUp() {}
void tearDown() {}

void test_idle_input_gives_no_event() {
  Debounce d(50);
  for (uint32_t t = 0; t < 1000; t += 10) TEST_ASSERT_FALSE(d.update(true, t));
}

void test_bounce_then_stable_press_gives_one_event() {
  Debounce d(50);
  const bool bounce[] = {false, true, false, true, false};   // дребезг в первые 5 мс
  for (uint32_t t = 0; t < 5; ++t) TEST_ASSERT_FALSE(d.update(bounce[t], t));
  TEST_ASSERT_FALSE(d.update(false, 53));                     // ещё 49 мс -- рано
  TEST_ASSERT_TRUE(d.update(false, 54));                      // 50 мс устойчивого уровня
  TEST_ASSERT_FALSE(d.update(false, 60));                     // событие не повторяется
}

void test_short_glitch_is_ignored() {
  Debounce d(50);
  TEST_ASSERT_FALSE(d.update(false, 10));                     // помеха...
  TEST_ASSERT_FALSE(d.update(true, 20));                      // ...пропала через 10 мс
  TEST_ASSERT_FALSE(d.update(true, 200));
}

void test_release_gives_no_event() {
  Debounce d(50);
  d.update(false, 0);
  TEST_ASSERT_TRUE(d.update(false, 50));                      // нажали
  d.update(true, 100);
  TEST_ASSERT_FALSE(d.update(true, 150));                     // отпустили: события нет
  d.update(false, 200);
  TEST_ASSERT_TRUE(d.update(false, 250));                     // и снова нажали
}

static int runTests() {
  UNITY_BEGIN();
  RUN_TEST(test_idle_input_gives_no_event);
  RUN_TEST(test_bounce_then_stable_press_gives_one_event);
  RUN_TEST(test_short_glitch_is_ignored);
  RUN_TEST(test_release_gives_no_event);
  return UNITY_END();
}

#ifdef ARDUINO
#include <Arduino.h>
void setup() { delay(2000); runTests(); }
void loop() {}
#else
int main(int, char**) { return runTests(); }
#endif
