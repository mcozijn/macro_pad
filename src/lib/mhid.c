#include "mhid.h"

static uint32_t blink_interval_ms = BLINK_NOT_MOUNTED;

static void send_hid_report(uint8_t report_id, int32_t btn) {
    if (!tud_hid_ready()) return;

    switch (report_id) {
        case REPORT_ID_KEYBOARD: {
            // Avoid sending multiple consecutive zero reports for keyboard
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
            // Avoid sending multiple consecutive zero reports
            static bool has_consumer_key = false;

            if (btn > 0) {
                uint16_t usage_code = (uint16_t)btn;
                tud_hid_report(REPORT_ID_CONSUMER_CONTROL, &usage_code, sizeof(usage_code));
                has_consumer_key = true;
            } else {
                if (has_consumer_key) tud_hid_report(REPORT_ID_CONSUMER_CONTROL, NULL, 0);
                has_consumer_key = false;
            }
        } break;
    }
}

void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id,
                           hid_report_type_t report_type, uint8_t const* buffer,
                           uint16_t bufsize) {
    (void)instance;

    if (report_type == HID_REPORT_TYPE_OUTPUT) {
        // Set keyboard LED (e.g., Caps Lock, Num Lock)
        if (report_id == REPORT_ID_KEYBOARD) {
            if (bufsize < 1) return;

            uint8_t const kbd_leds = buffer[0];

            if (kbd_leds & KEYBOARD_LED_CAPSLOCK) {
                // Caps Lock On: disable blink, turn LED on
                blink_interval_ms = 0;
                board_led_write(true);
            } else {
                // Caps Lock Off: back to normal blink
                board_led_write(false);
                blink_interval_ms = BLINK_MOUNTED;
            }
        }
    }
}

void hid_task(run_hid_options options) {
    int32_t const btn = options.get_key ? options.get_key() : -1;
    int32_t const enc = options.get_enc ? options.get_enc() : -1;

    if (tud_suspended() && (btn != -1 || enc != -1)) {
        tud_remote_wakeup();
    } else {
        send_hid_report(REPORT_ID_KEYBOARD, btn);
        send_hid_report(REPORT_ID_CONSUMER_CONTROL, enc);
        if (options.set_dpy) options.set_dpy((int8_t)btn);
    }
}

uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id,
                               hid_report_type_t report_type, uint8_t* buffer,
                               uint16_t reqlen) {
    (void)instance;
    (void)report_id;
    (void)report_type;
    (void)buffer;
    (void)reqlen;
    return 0;
}