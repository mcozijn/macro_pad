#include "config.h"
#include "mhid.h"
#include "pico/stdlib.h"

void setup_gpio() {
   for (uint8_t i = 0; i < MATRIX_COLS; i++) {
      gpio_init(col_pins[i]);
      gpio_set_dir(col_pins[i], GPIO_IN);
      gpio_pull_up(col_pins[i]);
      for (uint8_t j = 0; j < MATRIX_ROWS; j++) {
         gpio_init(row_pins[j]);
         gpio_set_dir(row_pins[j], GPIO_OUT);
      }
   }
}

int8_t scan_matrix() {
   int8_t key = 0;
   for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
      gpio_put(row_pins[i], 0);
      sleep_us(1);
      for (uint8_t j = 0; j < MATRIX_COLS; j++) {
         if (gpio_get(col_pins[j]) == 0) {
            key = (key == 0) ? keymap[i][MATRIX_COLS - j - 1] : -1;
         }
      }
      gpio_put(row_pins[i], 1);
   }
   return key;
}

int8_t get_key() {
   int8_t key = -1;
   uint32_t start = to_ms_since_boot(get_absolute_time());
   while (to_ms_since_boot(get_absolute_time()) - start < 10) {
      if (key != scan_matrix()) {
         key = scan_matrix();
      }
   }
   return key;
}

int main() {
   board_init();
   tud_init(BOARD_TUD_RHPORT);

   if (board_init_after_tusb) {
      board_init_after_tusb();
   }
   setup_gpio();
   while (1) {
      tud_task();  // tinyusb device task
      led_blinking_task();
      hid_task(get_key);
   }
}
