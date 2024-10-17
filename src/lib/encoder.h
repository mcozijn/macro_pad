#pragma once

#include <hardware/gpio.h>
#include <hardware/sync.h>
#include <pico/stdlib.h>

#include "config.h"

#define ENC_S_A (1 << ENC_A)                            // signal on pin A
#define ENC_S_B (1 << ENC_B)                            // signal on pin B
#define ENC_S_BOTH (ENC_S_A | ENC_S_B)                  // signal on both pins
#define ENC_S_ALL (ENC_S_A | ENC_S_B | (1 << ENC_BTN))  // signal on all pins
#define ENC_STEPS 255

enum {
   NONE = -1,
   INCREMENT,
   BUTTON,
   DECREMENT,
};

void setup_enc_gpio();
int8_t get_enc_pos_diff();
uint8_t get_enc_btn_state();