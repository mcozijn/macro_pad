#include "encoder.h"

volatile uint32_t enc_direction = 0;
volatile int8_t enc_pos = 0;
volatile bool enc_btn_state = 0;

void enc_irq(uint gpio, uint32_t events) {
    uint32_t irq_status = save_and_disable_interrupts();

    switch (gpio) {
        // rotation
        case ENC_LEFT ... ENC_RIGHT:
            handle_enc_spin(gpio_get_all() & ENC_BOTH_MASK);
            break;

        // button
        case ENC_BTN:
            enc_btn_state = (events & GPIO_IRQ_EDGE_FALL) ?: false;
            break;
    }
    restore_interrupts(irq_status);
}

void handle_enc_spin(uint32_t mask) {
    // if any of the pins are up
    if (mask & ENC_BOTH_MASK) {
        enc_direction = mask;

    // else, if we have a direction
    } else if (enc_direction) {
        if (enc_direction & ENC_LEFT_MASK) {
            enc_pos = 1;
        } else if (enc_direction & ENC_RIGHT_MASK) {
            enc_pos = -1;
        }

        // reset those pins
        enc_direction &= (uint32_t)~ENC_BOTH_MASK;
    }
}

void setup_enc_gpio() {
    gpio_init_mask(ENC_ALL_MASK);
    gpio_set_dir_in_masked(ENC_ALL_MASK);
    gpio_pull_up(ENC_LEFT);
    gpio_pull_up(ENC_RIGHT);
    gpio_pull_up(ENC_BTN);
    gpio_set_irq_enabled_with_callback(ENC_RIGHT, GPIO_IRQ_EDGE_FALL, true, &enc_irq);
    gpio_set_irq_enabled_with_callback(ENC_LEFT, GPIO_IRQ_EDGE_FALL, true, &enc_irq);
    gpio_set_irq_enabled_with_callback(ENC_BTN, GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE, true, &enc_irq);
}

int8_t get_enc_pos_diff() {
    int8_t tmp = enc_pos;

    // reset the 
    enc_pos = 0;

    return tmp;
}

bool get_enc_btn_state() {
    return enc_btn_state;
}
