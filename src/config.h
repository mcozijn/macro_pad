#pragma once
#include <pico/stdlib.h>

#include "tusb.h"

#define DEBOUNCE_DELAY 10
#define MATRIX_ROWS 4
#define MATRIX_COLS 3

#define query(key) keymap[key / MATRIX_COLS][key % MATRIX_COLS]

typedef struct EncoderControls {
    uint16_t increment;
    uint16_t decrement;
    uint16_t button;
} EncoderControls;

static const uint col_pins[MATRIX_COLS] = {3, 4, 5};
static const uint row_pins[MATRIX_ROWS] = {6, 7, 8, 9};

typedef struct Key {
    bool type;
    union {
        uint8_t mod_key;
        uint8_t keys[6];
    };
}Key;

static const Key keymap[MATRIX_ROWS][MATRIX_COLS] = {
    {
        {true, .mod_key = KEYBOARD_MODIFIER_LEFTGUI},
        {false, .keys = {HID_KEY_A, HID_KEY_B, HID_KEY_C, HID_KEY_D, HID_KEY_E, HID_KEY_F}},
        {false, .keys = {HID_KEY_SHIFT_LEFT, HID_KEY_S}},
    },
    {
        {true, .mod_key = KEYBOARD_MODIFIER_LEFTCTRL},
        {false, .keys = {HID_KEY_GUI_LEFT, HID_KEY_ARROW_RIGHT}},
        {false, .keys = {HID_KEY_GUI_LEFT, HID_KEY_ARROW_LEFT}},
    },
    {
        {true, .mod_key = KEYBOARD_MODIFIER_LEFTALT},
        {false, .keys = {HID_KEY_Y, HID_KEY_Z, HID_KEY_1, HID_KEY_2, HID_KEY_3, HID_KEY_4}},
        {false, .keys = {HID_KEY_5, HID_KEY_6, HID_KEY_7, HID_KEY_8, HID_KEY_9, HID_KEY_0}},
    },
    {
        {true, .mod_key = KEYBOARD_MODIFIER_LEFTGUI},
        {false, .keys = {HID_KEY_T, HID_KEY_U, HID_KEY_V, HID_KEY_W, HID_KEY_X, HID_KEY_Y}},
        {false, .keys = {HID_KEY_Z, HID_KEY_1, HID_KEY_2, HID_KEY_3, HID_KEY_4, HID_KEY_5}},
    },
};



// static const Key keymap[MATRIX_ROWS][MATRIX_COLS] = {
//     {
//         {true, .mod_key = KEYBOARD_MODIFIER_LEFTALT}, // Alt key  (ROW 1, COL 3)
//         {true, .mod_key = KEYBOARD_MODIFIER_LEFTCTRL}, // Ctrl key (ROW 1, COL 2)
//         {true, .mod_key = KEYBOARD_MODIFIER_LEFTGUI}, // Windows key (ROW 1, COL 1)
//     },
//     {
//         {false, .keys = {HID_KEY_C}}, // C (R2:C3)
//         {false, .keys = {HID_KEY_B}}, // B (R2:C2)
//         {false, .keys = {HID_KEY_A}}, // A (R2:C1)
//     },
//     {
//         {false, .keys = {HID_KEY_F}}, // F (R3:C3)
//         {false, .keys = {HID_KEY_E}}, // E (R3:C2)
//         {false, .keys = {HID_KEY_D}}, // D (R3:C1)
//     },
//     {
//         {true, .mod_key = KEYBOARD_MODIFIER_LEFTSHIFT}, // Shift key (R4:C3)
//         {false, .keys = {HID_KEY_G}}, // G (R4:C2)
//         {true, .mod_key = KEYBOARD_MODIFIER_LEFTSHIFT}, // Shift key (R4:C1)
//     },
// };

// Simple encoder example
static const EncoderControls encoder = {HID_USAGE_CONSUMER_VOLUME_INCREMENT, HID_USAGE_CONSUMER_VOLUME_DECREMENT, HID_USAGE_CONSUMER_PLAY_PAUSE};