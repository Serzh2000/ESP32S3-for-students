#include <Arduino.h>
// Two external 3.3V-logic CAN transceivers; 500 kbit/s on both boards.
#include <Arduino.h>
#include "driver/twai.h"
const uint32_t NODE_ID = 0x101; // second board: 0x102
bool ready = false;
void setup() {
  Serial.begin(115200);
  twai_general_config_t general = TWAI_GENERAL_CONFIG_DEFAULT(GPIO_NUM_16, GPIO_NUM_17,
                                            TWAI_MODE_NORMAL);
  twai_timing_config_t timing = TWAI_TIMING_CONFIG_500KBITS();
  twai_filter_config_t filter = TWAI_FILTER_CONFIG_ACCEPT_ALL();
  ESP_ERROR_CHECK(twai_driver_install(&general, &timing, &filter));
  ESP_ERROR_CHECK(twai_start()); ready = true;
}
void loop() {
  if (!ready) return;
  static uint32_t last = 0, sequence = 0;
  if (millis() - last >= 1000) {
    last = millis();
    twai_message_t tx = {}; tx.identifier = NODE_ID;
    tx.data_length_code = 4;
    for (int i = 0; i < 4; ++i) tx.data[i] = (sequence >> (8 * i)) & 255;
    esp_err_t err = twai_transmit(&tx, pdMS_TO_TICKS(20));
    Serial.printf("queue result=%s seq=%lu\n", esp_err_to_name(err),
                  (unsigned long)sequence++);
  }
  twai_message_t rx;
  if (twai_receive(&rx, pdMS_TO_TICKS(10)) == ESP_OK)
    Serial.printf("RX id=%03lx bytes=%d first=%d\n",
        (unsigned long)rx.identifier, rx.data_length_code, rx.data[0]);
  twai_status_info_t status;
  if (twai_get_status_info(&status) == ESP_OK &&
      status.state == TWAI_STATE_BUS_OFF) {
    Serial.println("BUS OFF: check wiring, then reset");
    ready = false;
  }
}
