#include "hid_helpers.h"
#include "mhid.h"

// Initialize the mapping arrays using the macros from "tusb.h"
uint8_t const ascii_to_keycode[128][2] = {HID_ASCII_TO_KEYCODE};
uint8_t const keycode_to_ascii[128][2] = {HID_KEYCODE_TO_ASCII};

/**
 * @brief Convert an ASCII character to HID keycode and modifier.
 *
 * @param chr The ASCII character to convert.
 * @param keycode Pointer to store the HID keycode.
 * @param modifier Pointer to store the HID modifier (e.g., shift).
 * @return true if successful, false if character is unsupported.
 */
bool char_to_keycode(char chr, uint8_t *keycode, uint8_t *modifier) {
    uint8_t chr_uint = (uint8_t)chr;

    // Check if the character is within the valid ASCII range
    if (chr_uint >= 128) {
        return false;
    }

    // Get the shift flag and keycode from the mapping array
    uint8_t shift = ascii_to_keycode[chr_uint][0];
    uint8_t kc = ascii_to_keycode[chr_uint][1];

    if (kc == 0) {
        // Unsupported character
        return false;
    }

    // Set modifier and keycode
    *modifier = shift ? KEYBOARD_MODIFIER_LEFTSHIFT : 0;
    *keycode = kc;

    return true;
}

/**
 * @brief Convert a TinyUSB keycode and shift flag to an ASCII character.
 *
 * @param keycode The TinyUSB HID keycode to convert.
 * @param shift Indicates whether the shift modifier is active.
 * @return The corresponding ASCII character, or 0 if unsupported.
 */
char keycode_to_char(uint8_t keycode, bool shift) {
    if (keycode >= 128) return 0;
    return keycode_to_ascii[keycode][shift ? 1 : 0];
}

// /**
//  * @brief Send a sequence of keycodes to the host.
//  * 
//  * @param report_id The report ID to use.
//  * @param modifier The modifier byte.
//  * @param keycodes The array of keycodes to send.
//  */

// void send_keycodes(uint8_t report_id, uint8_t modifier, uint8_t keycodes[6]) {
//     tud_hid_keyboard_report(report_id, modifier, keycodes);
//     sleep_ms(10);

//     // Send a null report to release all keys
//     tud_hid_keyboard_report(report_id, 0, NULL);
//     sleep_ms(10);
// }


// void send_long_keystroke(const uint8_t *sequence, size_t length, uint8_t modifier) {
//     for (size_t i = 0; i < length; i++) {
//         uint8_t keycode = sequence[i];
//         send_keycodes(REPORT_ID_KEYBOARD, modifier, &keycode);
//     }
// }

// void send_string(const char *str, uint8_t modifier) {
//     size_t length = strlen(str);
//     for (size_t i = 0; i < length; i++) {
//         char chr = str[i];
//         uint8_t keycode;
//         uint8_t modifier;
//         if (char_to_keycode(chr, &keycode, &modifier)) {
//             send_keycodes(REPORT_ID_KEYBOARD, modifier, &keycode);
//         }
//     }
// }