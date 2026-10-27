// Read-only inspection: no flash erase and no eFuse programming.
#include <Arduino.h>
#include "esp_ota_ops.h"
#include "esp_heap_caps.h"
#include "esp_system.h"
void setup() {
  Serial.begin(115200); delay(1000);
  const esp_partition_t* running = esp_ota_get_running_partition();
  const esp_partition_t* next = esp_ota_get_next_update_partition(nullptr);
  Serial.printf("reset=%d app=%s size=%lu next_OTA=%s\n",
    (int)esp_reset_reason(), running ? running->label : "?",
    running ? (unsigned long)running->size : 0UL,
    next ? next->label : "none");
  Serial.printf("Flash=%lu PSRAM=%lu IDF=%s\n",
    (unsigned long)ESP.getFlashChipSize(),
    (unsigned long)ESP.getPsramSize(), esp_get_idf_version());
}
void loop() {
  Serial.printf("heap=%u largest_internal=%u minimum=%u\n",
    (unsigned)esp_get_free_heap_size(),
    (unsigned)heap_caps_get_largest_free_block(MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT),
    (unsigned)esp_get_minimum_free_heap_size());
  delay(5000);
}
