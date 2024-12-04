#pragma once
#include <pico/stdlib.h>

#include "tusb.h"
#include "tllist.h"

#define DEBOUNCE_DELAY 10
#define MATRIX_ROWS 4
#define MATRIX_COLS 3
<<<<<<< Updated upstream
#define RESET_BOOTSEL_KEY 2
=======
#define MOMENTARY_SIZE 20
#define KEY 0
#define MOD 1
#define CONSUMER 2
>>>>>>> Stashed changes

#define query(key) keymap[key / MATRIX_COLS][key % MATRIX_COLS]

static const uint col_pins[MATRIX_COLS] = {3, 4, 5};
static const uint row_pins[MATRIX_ROWS] = {6, 7, 8, 9};

typedef struct EncoderControls {
    uint16_t increment;
    uint16_t decrement;
    uint16_t button;
} EncoderControls;

typedef struct Key {
    uint type;
    union {
        uint8_t mod_key;
        uint8_t keys[6];
        uint16_t consumer_key;
    };
}Key;

static const Key keymap[MATRIX_ROWS][MATRIX_COLS] = {
    {
        {MOD, .mod_key = KEYBOARD_MODIFIER_LEFTGUI},
        {KEY, .keys = {HID_KEY_A, HID_KEY_B, HID_KEY_C, HID_KEY_D, HID_KEY_E, HID_KEY_F}},
        {CONSUMER, .consumer_key = HID_USAGE_CONSUMER_MUTE},
    },
    {
        {MOD, .mod_key = KEYBOARD_MODIFIER_LEFTCTRL},
        {KEY, .keys = {HID_KEY_GUI_LEFT, HID_KEY_ARROW_RIGHT}},
        {KEY, .keys = {HID_KEY_GUI_LEFT, HID_KEY_ARROW_LEFT}},
    },
    {
        {MOD, .mod_key = KEYBOARD_MODIFIER_LEFTALT},
        {KEY, .keys = {HID_KEY_Y, HID_KEY_Z, HID_KEY_1, HID_KEY_2, HID_KEY_3, HID_KEY_4}},
        {KEY, .keys = {HID_KEY_5, HID_KEY_6, HID_KEY_7, HID_KEY_8, HID_KEY_9, HID_KEY_0}},
    },
    {
        {MOD, .mod_key = KEYBOARD_MODIFIER_LEFTGUI},
        {KEY, .keys = {HID_KEY_T, HID_KEY_U, HID_KEY_V, HID_KEY_W, HID_KEY_X, HID_KEY_Y}},
        {KEY, .keys = {HID_KEY_Z, HID_KEY_1, HID_KEY_2, HID_KEY_3, HID_KEY_4, HID_KEY_5}},
    },
};

// Simple encoder example
static const EncoderControls encoder = {
    HID_USAGE_CONSUMER_VOLUME_INCREMENT, // Spinning clockwise
    HID_USAGE_CONSUMER_VOLUME_DECREMENT, // Spinning counter-clockwise
    HID_USAGE_CONSUMER_PLAY_PAUSE // Pressing the encoder
};



//Momentary. Firware will wait until key is released and pressed again before sending new hid report
extern const uint16_t momentary[] =
{
  HID_USAGE_CONSUMER_POWER,
  HID_USAGE_CONSUMER_MUTE,
  HID_USAGE_CONSUMER_BASS_BOOST,
  HID_USAGE_CONSUMER_RESET,
  HID_USAGE_CONSUMER_SLEEP,
  HID_USAGE_CONSUMER_PLAY_PAUSE,
  HID_USAGE_CONSUMER_SCAN_NEXT,
  HID_USAGE_CONSUMER_SCAN_PREVIOUS,
  HID_USAGE_CONSUMER_STOP,
  HID_USAGE_CONSUMER_AL_CONSUMER_CONTROL_CONFIGURATION,
  HID_USAGE_CONSUMER_AL_EMAIL_READER,
  HID_USAGE_CONSUMER_AL_CALCULATOR,
  HID_USAGE_CONSUMER_AL_LOCAL_BROWSER,
  HID_USAGE_CONSUMER_AC_SEARCH,
  HID_USAGE_CONSUMER_AC_HOME,
  HID_USAGE_CONSUMER_AC_BACK,
  HID_USAGE_CONSUMER_AC_FORWARD,
  HID_USAGE_CONSUMER_AC_STOP,
  HID_USAGE_CONSUMER_AC_REFRESH,
  HID_USAGE_CONSUMER_AC_BOOKMARKS,
};

