#include "encoder.h"

volatile uint8_t enc_a_state = 0;
volatile uint8_t enc_b_state = 0;
volatile uint8_t enc_pos = 0;
volatile uint8_t old_enc_pos = 0;
volatile uint8_t enc_btn_state = 0;

void enc_irq(uint gpio, uint32_t events) {
    uint32_t irq_status = save_and_disable_interrupts();
    uint32_t curr_mask = gpio_get_all() & ENC_S_BOTH;
    if ((curr_mask == 0x00) && (enc_a_state || enc_b_state)) {
        if (enc_a_state && enc_pos > 0) {
            enc_pos--;
        } else if (enc_b_state && enc_pos < (ENC_STEPS - 1)) {
            enc_pos++;
        }
        enc_a_state = 0;
        enc_b_state = 0;
    } else if (curr_mask == ENC_S_A) {
        enc_a_state = 1;
    } else if (curr_mask == ENC_S_B) {
        enc_b_state = 1;
    }
    restore_interrupts(irq_status);
}

void enc_btn_irq(uint gpio, uint32_t events) {
    uint32_t irq_status = save_and_disable_interrupts();
    enc_btn_state = (events & GPIO_IRQ_EDGE_FALL) ? 1 : 0;
    restore_interrupts(irq_status);
}

void setup_enc_gpio() {
    gpio_init_mask(ENC_S_ALL);
    gpio_set_dir_in_masked(ENC_S_ALL);
    gpio_pull_up(ENC_A);
    gpio_pull_up(ENC_B);
    gpio_set_irq_enabled_with_callback(ENC_B, GPIO_IRQ_EDGE_FALL, true, &enc_irq);
    gpio_set_irq_enabled_with_callback(ENC_A, GPIO_IRQ_EDGE_FALL, true, &enc_irq);
}

// getters
int8_t get_enc_pos_diff() {
    uint8_t tmp = old_enc_pos;
    old_enc_pos = enc_pos;
    return (int8_t)(enc_pos - tmp);
}

uint8_t get_enc_btn_state() {
    return enc_btn_state;
}
