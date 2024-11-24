#pragma once
#include <pico/stdlib.h>

#include "tusb.h"

#define DEBOUNCE_DELAY 10
#define MATRIX_ROWS 4
#define MATRIX_COLS 3

typedef struct EncoderControls {
    uint16_t increment;
    uint16_t decrement;
    uint16_t button;
} EncoderControls;

static const uint col_pins[MATRIX_COLS] = {3, 4, 5};
static const uint row_pins[MATRIX_ROWS] = {6, 7, 8, 9};

static const uint8_t keymap[MATRIX_ROWS][MATRIX_COLS] = {
    {HID_KEY_A, HID_KEY_B, HID_KEY_C},
    {HID_KEY_D, HID_KEY_E, HID_KEY_F},
    {HID_KEY_G, HID_KEY_H, HID_KEY_I},
    {HID_KEY_J, HID_KEY_K, HID_KEY_L},
};

static const EncoderControls encoder = {HID_USAGE_CONSUMER_VOLUME_INCREMENT, HID_USAGE_CONSUMER_VOLUME_DECREMENT, HID_USAGE_CONSUMER_PLAY_PAUSE};