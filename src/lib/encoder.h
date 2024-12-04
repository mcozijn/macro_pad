#pragma once

#include <hardware/gpio.h>
#include <hardware/sync.h>

#include "pins.h"

#define ENC_PIN_AB 16
#define ENC_SM_ADDR 0

void setup_enc();
void handle_enc_spin();
int8_t get_enc_pos_diff();
void reset_enc_btn_state();
bool get_enc_btn_state();