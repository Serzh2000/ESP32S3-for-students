#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
volatile float observed;
__attribute__((noinline)) float meanWrong(int total, int count) {
    return total / count;
}
void app_main(void) {
    while (1) {
        observed = meanWrong(5, 2);
        printf("mean=%.2f\n", (double)observed);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
