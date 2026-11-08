// Signal only: GPIO6. Power servo from an appropriate external supply.
#include <Arduino.h>
#include "driver/mcpwm_prelude.h"
void setup() {
  Serial.begin(115200);
  mcpwm_timer_handle_t timer = nullptr;
  mcpwm_timer_config_t tc = {};
  tc.group_id = 0; tc.clk_src = MCPWM_TIMER_CLK_SRC_DEFAULT;
  tc.resolution_hz = 1000000; tc.period_ticks = 20000;
  tc.count_mode = MCPWM_TIMER_COUNT_MODE_UP;
  ESP_ERROR_CHECK(mcpwm_new_timer(&tc, &timer));
  mcpwm_oper_handle_t oper = nullptr;
  mcpwm_operator_config_t oc = {}; oc.group_id = 0;
  ESP_ERROR_CHECK(mcpwm_new_operator(&oc, &oper));
  ESP_ERROR_CHECK(mcpwm_operator_connect_timer(oper, timer));
  mcpwm_cmpr_handle_t cmp = nullptr;
  mcpwm_comparator_config_t cc = {}; cc.flags.update_cmp_on_tez = true;
  ESP_ERROR_CHECK(mcpwm_new_comparator(oper, &cc, &cmp));
  ESP_ERROR_CHECK(mcpwm_comparator_set_compare_value(cmp, 1500));
  mcpwm_gen_handle_t gen = nullptr;
  mcpwm_generator_config_t gc = {}; gc.gen_gpio_num = 6;
  ESP_ERROR_CHECK(mcpwm_new_generator(oper, &gc, &gen));
  ESP_ERROR_CHECK(mcpwm_generator_set_action_on_timer_event(gen,
      MCPWM_GEN_TIMER_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP,
      MCPWM_TIMER_EVENT_EMPTY, MCPWM_GEN_ACTION_HIGH)));
  ESP_ERROR_CHECK(mcpwm_generator_set_action_on_compare_event(gen,
      MCPWM_GEN_COMPARE_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP,
      cmp, MCPWM_GEN_ACTION_LOW)));
  ESP_ERROR_CHECK(mcpwm_timer_enable(timer));
  ESP_ERROR_CHECK(mcpwm_timer_start_stop(timer, MCPWM_TIMER_START_NO_STOP));
}
void loop() { delay(100); }
