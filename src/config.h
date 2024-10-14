#pragma once
#include <pico/stdlib.h>

#include "tusb.h"

#define MATRIX_ROWS 4
#define MATRIX_COLS 3

static const int8_t col_pins[MATRIX_COLS] = {3, 4, 5};
static const int8_t row_pins[MATRIX_ROWS] = {6, 7, 8, 9};

static const int8_t keymap[MATRIX_ROWS][MATRIX_COLS] = {
    {HID_KEY_1, HID_KEY_2, HID_KEY_3},
    {HID_KEY_4, HID_KEY_5, HID_KEY_6},
    {HID_KEY_7, HID_KEY_8, HID_KEY_9},
    {HID_KEY_0, HID_KEY_ENTER, HID_KEY_BACKSPACE},
};
