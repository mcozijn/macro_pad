#include "mhid.h"

#include "config.h"
#include "encoder.h"

static uint32_t blink_interval_ms = BLINK_NOT_MOUNTED;

static void send_hid_report(uint8_t report_id, int32_t btn) {
   if (!tud_hid_ready()) return;
   switch (report_id) {
      case REPORT_ID_KEYBOARD: {
         // use to avoid send multiple consecutive zero report for keyboard
         static bool has_keyboard_key = false;

         if (btn > 0) {
            uint8_t keycode[6] = {0};
            keycode[0] = (uint8_t)btn;
            tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, keycode);
            has_keyboard_key = true;
         } else {
            if (has_keyboard_key) tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, NULL);
            has_keyboard_key = false;
         }
      } break;
      case REPORT_ID_CONSUMER_CONTROL: {
         // use to avoid send multiple consecutive zero report
         static bool has_consumer_key = false;

         switch (btn) {
            case DECREMENT: {
               uint16_t volume_down = encoder.decrement;
               tud_hid_report(REPORT_ID_CONSUMER_CONTROL, &volume_down, 2);
               has_consumer_key = true;
            } break;
            case INCREMENT: {
               uint16_t volume_up = encoder.increment;
               tud_hid_report(REPORT_ID_CONSUMER_CONTROL, &volume_up, 2);
               has_consumer_key = true;
            } break;
            case BUTTON: {
               uint16_t play_pause = encoder.button;
               tud_hid_report(REPORT_ID_CONSUMER_CONTROL, &play_pause, 2);
               has_consumer_key = true;
            } break;
            default: {
               if (has_consumer_key) tud_hid_report(REPORT_ID_CONSUMER_CONTROL, NULL, 0);
               has_consumer_key = false;
            } break;
         }
      } break;
   }
}

void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const* buffer, uint16_t bufsize) {
   (void)instance;

   if (report_type == HID_REPORT_TYPE_OUTPUT) {
      // Set keyboard LED e.g Capslock, Numlock etc...
      if (report_id == REPORT_ID_KEYBOARD) {
         // bufsize should be (at least) 1
         if (bufsize < 1) return;

         uint8_t const kbd_leds = buffer[0];

         if (kbd_leds & KEYBOARD_LED_CAPSLOCK) {
            // Capslock On: disable blink, turn led on
            blink_interval_ms = 0;
            board_led_write(true);
         } else {
            // Caplocks Off: back to normal blink
            board_led_write(false);
            blink_interval_ms = BLINK_MOUNTED;
         }
      }
   }
}

void led_blinking_task() {
   static uint32_t start_ms = 0;
   static bool led_state = false;

   if (!blink_interval_ms) return;

   if (board_millis() - start_ms < blink_interval_ms) return;
   start_ms += blink_interval_ms;

   board_led_write(led_state);
   led_state = 1 - led_state;
}

void hid_task(get_key_fn get_key, get_enc_fn get_enc) {
   const uint32_t interval_ms = 10;
   static uint32_t start_ms = 0;

   if (board_millis() - start_ms < interval_ms) return;
   start_ms += interval_ms;

   int32_t const btn = get_key();
   int32_t const enc = get_enc();

   if (tud_suspended() && (btn || enc)) {
      tud_remote_wakeup();
   } else {
      send_hid_report(REPORT_ID_KEYBOARD, btn);
      send_hid_report(REPORT_ID_CONSUMER_CONTROL, enc);
   }
}

uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t* buffer, uint16_t reqlen) { return 0; }
