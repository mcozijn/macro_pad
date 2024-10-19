#include "config.h"
#include "encoder.h"
#include "hardware/i2c.h"
#include "hid_helpers.h"
#include "mhid.h"
#include "pico/stdlib.h"
#include "ssd1306.h"
#include "tusb.h"

ssd1306_t oled_display;
int8_t encoder_pos = 0;
int8_t last_encoder_pos = 0;

void setup_i2c() {
    i2c_init(I2C_INSTANCE(1), 400 * 1000);
    gpio_set_function(SCREEN_SDA, GPIO_FUNC_I2C);
    gpio_set_function(SCREEN_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(SCREEN_SDA);
    gpio_pull_up(SCREEN_SCL);
}

void setup_kb_gpio() {
    for (uint8_t i = 0; i < MATRIX_COLS; i++) {
        gpio_init(col_pins[i]);
        gpio_set_dir(col_pins[i], GPIO_IN);
        gpio_pull_up(col_pins[i]);
    }
    for (uint8_t j = 0; j < MATRIX_ROWS; j++) {
        gpio_init(row_pins[j]);
        gpio_set_dir(row_pins[j], GPIO_OUT);
        gpio_put(row_pins[j], 1);
    }
}

int8_t scan_matrix() {
    int8_t key = 0;
    for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
        gpio_put(row_pins[i], 0);
        sleep_us(1);
        for (uint8_t j = 0; j < MATRIX_COLS; j++) {
            if (gpio_get(col_pins[j]) == 0) {
                key = (key == 0) ? (int8_t)keymap[i][MATRIX_COLS - j - 1] : -1;
            }
        }
        gpio_put(row_pins[i], 1);
    }
    return key;
}

int8_t get_key() {
    int8_t key = -1;
    uint32_t start = to_ms_since_boot(get_absolute_time());
    while (to_ms_since_boot(get_absolute_time()) - start < DEBOUNCE_DELAY) {
        int8_t current_key = scan_matrix();
        if (key != current_key) {
            key = current_key;
            start = to_ms_since_boot(get_absolute_time());
        }
    }
    return key;
}

int8_t get_enc() {
    if (get_enc_pos_diff() > 0) {
        return INCREMENT;  // increment
    } else if (get_enc_pos_diff() < 0) {
        return DECREMENT;  // decrement
    } else if (get_enc_btn_state()) {
        return BUTTON;  // button
    }
    return NONE;
    // return INCREMENT;
}

void set_dpy(int8_t keycode) {
    if (keycode == -1) return;
    char ch = keycode_to_char((char)keycode, false);
    if (ch) {
        ssd1306_clear(&oled_display);
        char str[2] = {ch, '\0'};
        ssd1306_draw_string(&oled_display, 0, 0, 7, str);
        ssd1306_show(&oled_display);
    }
}

int main() {
    board_init();
    tud_init(BOARD_TUD_RHPORT);

    if (board_init_after_tusb) {
        board_init_after_tusb();
    }

    // Display setup
    setup_i2c();
    ssd1306_init(&oled_display, 128, 64, 0x3C, I2C_INSTANCE(1));
    ssd1306_clear(&oled_display);
    ssd1306_show(&oled_display);

    // Keyboard and encoder setup
    setup_kb_gpio();
    setup_enc_gpio();

    while (1) {
        tud_task();
        hid_task(get_key, get_enc, set_dpy);
    }
}
