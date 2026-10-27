#include <stdint.h>
#include "ulp_riscv.h"
#include "ulp_riscv_utils.h"
#include "ulp_riscv_gpio.h"
uint32_t consecutive = 0; // RTC slow memory, retained between timer starts
int main(void) {
    if (ulp_riscv_gpio_get_level(GPIO_NUM_4) == 0) ++consecutive;
    else consecutive = 0;
    if (consecutive >= 10) {
        consecutive = 0;
        ulp_riscv_wakeup_main_processor();
    }
    return 0; // automatic halt; next timer period starts another scan
}
