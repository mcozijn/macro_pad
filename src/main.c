#include "main.h"

/*
    USEFUL FUNCTIONS:

        DISPLAY:
            display_clear(&oled_display);           // clears the display
            display_draw_string(&oled_display, )    // draws a string on the display
            display_show(&oled_display)             // submits to the display 
*/

/*
 *
 *
 * 
 */
void set_display(int8_t keycode) {
    if (keycode == -1) return; // if nothing is pressed

    char ch = keycode_to_char((char)keycode, false);

    if (ch) {

        // Draw something to the screen!

        // example code:
        display_clear(&oled_display);
        char str[2] = {ch, '\0'};
        display_draw_string(&oled_display, 0, 0, 7, str);
        display_show(&oled_display);
    }
}

/*
 * todo comment this and say to modify it if you want
 * 
 * 
 */
hid_report get_keycode() {
    int8_t keys[2] = {0};
    hid_report report = {.valid = false};
    uint32_t start = to_ms_since_boot(get_absolute_time());

    while (to_ms_since_boot(get_absolute_time()) - start < DEBOUNCE_DELAY) {
        int8_t current_keys[2] = {0};
        int8_t cnt = 0;
        scan_matrix(current_keys, &cnt);

        if (memcmp(keys, current_keys, 2) != 0 && cnt > 0) {
            memcpy(keys, current_keys, 2);
            report = parse_keys(keys, cnt);
            start = to_ms_since_boot(get_absolute_time());
        }
    }

    return report;
}

/*
 * MATRIX SCAN CODE (double for loop)
 *
 * arr:    An array of detected pressed buttons.
 *         Coordinates are stored in format: x * MATRIX_COLS + y
 * 
 * cnt:    Amount of stored values. Increment every time you add a pressed key.
 */ 
void scan_matrix(int8_t *arr, int8_t *cnt) {

    // fill in code, ask for help if you don't know!

}

/* MAIN FUNCTION */
int main() {
    // start everything up
    init_keypad();
    init_display();

    while (1) {
        // poll the usb
        tud_task(); 

        // update the macropad stuff
        update_macropad((macropad_options) {
            .get_keycode_function = get_keycode,
            .set_display_function = set_display
        });
    }
}
