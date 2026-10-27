// GPIO4 (LEDC generator) -> jumper -> GPIO5 (PCNT input).
#include <Arduino.h>
#include "driver/pulse_cnt.h"
pcnt_unit_handle_t counter = nullptr;
void setup() {
  Serial.begin(115200);
  pcnt_unit_config_t unit = {};
  unit.low_limit = -30000; unit.high_limit = 30000;
  ESP_ERROR_CHECK(pcnt_new_unit(&unit, &counter));
  pcnt_glitch_filter_config_t filter = {};
  filter.max_glitch_ns = 1000;
  ESP_ERROR_CHECK(pcnt_unit_set_glitch_filter(counter, &filter));
  pcnt_chan_config_t input = {};
  input.edge_gpio_num = 5; input.level_gpio_num = -1;
  pcnt_channel_handle_t channel = nullptr;
  ESP_ERROR_CHECK(pcnt_new_channel(counter, &input, &channel));
  ESP_ERROR_CHECK(pcnt_channel_set_edge_action(channel,
      PCNT_CHANNEL_EDGE_ACTION_INCREASE, PCNT_CHANNEL_EDGE_ACTION_HOLD));
  ESP_ERROR_CHECK(pcnt_unit_enable(counter));
  ESP_ERROR_CHECK(pcnt_unit_clear_count(counter));
  ESP_ERROR_CHECK(pcnt_unit_start(counter));
  if (!ledcAttach(4, 1000, 8)) {
    Serial.println("LEDC init failed"); return;
  }
  ledcWrite(4, 128);
}
void loop() {
  static int previous = 0;
  static uint32_t last = millis();
  uint32_t now = millis();
  if (now - last < 1000) return;
  int current = 0;
  ESP_ERROR_CHECK(pcnt_unit_get_count(counter, &current));
  // Modular difference; valid if fewer than 30000 pulses per interval.
  int delta = current - previous;
  if (delta < 0) delta += 30000;
  Serial.printf("count:%d frequency:%.1f Hz\n", current,
                delta * 1000.0 / (now - last));
  previous = current; last = now;
}
