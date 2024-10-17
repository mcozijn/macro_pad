#pragma once
#include <pico/stdlib.h>

#include "tusb.h"

#define DEBOUNCE_DELAY 10

#define MATRIX_ROWS 4
#define MATRIX_COLS 3

#define ENC_A 0
#define ENC_B 1
#define ENC_BTN 2

static const uint col_pins[MATRIX_COLS] = {3, 4, 5};
static const uint row_pins[MATRIX_ROWS] = {6, 7, 8, 9};

static const uint8_t keymap[MATRIX_ROWS][MATRIX_COLS] = {
    {HID_KEY_1, HID_KEY_2, HID_KEY_3},
    {HID_KEY_4, HID_KEY_5, HID_KEY_6},
    {HID_KEY_7, HID_KEY_8, HID_KEY_9},
    {HID_KEY_0, HID_KEY_ENTER, HID_KEY_BACKSPACE},
};

typedef struct encoder_controls_ {
   uint16_t increment;
   uint16_t decrement;
   uint16_t button;
} encoder_controls;

static const encoder_controls encoder = {HID_USAGE_CONSUMER_VOLUME_DECREMENT, HID_USAGE_CONSUMER_VOLUME_INCREMENT, HID_USAGE_CONSUMER_PLAY_PAUSE};