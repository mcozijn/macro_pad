#pragma once

#include <hardware/gpio.h>
#include <hardware/sync.h>

#include "pins.h"

#ifdef ENC_USE_STATE_MACHINE
/*State machine code*/
#define DIR_NONE 0x0
#define DIR_CW 0x10
#define DIR_CCW 0x20

#define R_START 0x0
#define R_CW_FINAL 0x1
#define R_CW_BEGIN 0x2
#define R_CW_NEXT 0x3
#define R_CCW_BEGIN 0x4
#define R_CCW_FINAL 0x5
#define R_CCW_NEXT 0x6

#define ENC_LEFT_MASK (1 << ENC_LEFT)                                   // signal on pin A
#define ENC_RIGHT_MASK (1 << ENC_RIGHT)                                 // signal on pin B
#define ENC_BOTH_MASK (ENC_LEFT_MASK | ENC_RIGHT_MASK)                  // signal on both pins
#define ENC_ALL_MASK (ENC_LEFT_MASK | ENC_RIGHT_MASK | (1 << ENC_BTN))  // signal on all pins

#endif  // ENC_USE_STATE_MACHINE

#ifdef ENC_USE_PIO
#define ENC_PIN_AB 16
#define ENC_SM_ADDR 0
#endif  // ENC_USE_PIO

void setup_enc();
void handle_enc_spin();
int8_t get_enc_pos_diff();
bool get_enc_btn_state();