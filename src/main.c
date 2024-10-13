#include "hid.h"
#include "pico/stdlib.h"

int main() {
   setup_default_uart();
   printf("Hello, world!\n");
   board_init();

   tud_init(BOARD_TUD_RHPORT);

   if (board_init_after_tusb) {
      board_init_after_tusb();
   }

   while (1) {
      tud_task();  // tinyusb device task
      led_blinking_task();
      hid_task();
   }
   return 0;
}