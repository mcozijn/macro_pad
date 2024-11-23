#pragma once

#include <hardware/gpio.h>
#include <hardware/sync.h>

#include "pins.h"

#define ENC_LEFT_MASK (1 << ENC_LEFT)                       // signal on pin A
#define ENC_RIGHT_MASK (1 << ENC_RIGHT)                     // signal on pin B
#define ENC_BOTH_MASK (ENC_LEFT_MASK | ENC_RIGHT_MASK)      // signal on both pins
#define ENC_ALL_MASK (ENC_LEFT_MASK | ENC_RIGHT_MASK | (1 << ENC_BTN))   // signal on all pins

void setup_enc_gpio();
void handle_enc_spin();
int8_t get_enc_pos_diff();
bool get_enc_btn_state();