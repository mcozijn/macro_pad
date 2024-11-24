#define ENC_USE_PIO
#include "encoder.h"

#include <hardware/clocks.h>
#include <hardware/pio.h>

#include "qenc.pio.h"

#ifdef ENC_USE_STATE_MACHINE
static volatile uint32_t enc_direction = 0;
static volatile int8_t enc_pos = 0;
static volatile uint32_t state = R_START;
#endif

#ifdef ENC_USE_PIO
static int32_t new_value = 0;
static int32_t old_value = 0;
static int32_t delta = 0;
static PIO pio = pio0;
#endif

static volatile bool enc_btn_state = 0;

#ifdef ENC_USE_STATE_MACHINE



inline int8_t get_enc_pos_diff() {
    uint8_t pstate = (gpio_get(ENC_LEFT) << 1) | gpio_get(ENC_RIGHT);
    state = sm[state & 0xf][pstate];
    int8_t tmp = 0;
    tmp = (state & 0x30) == DIR_CW ? 1 : (state & 0x30) == DIR_CCW ? -1
                                                                   : 0;
    if (tmp) state = R_START;
    return tmp;
}

void setup_enc_gpio() {
    gpio_init_mask(ENC_ALL_MASK);
    gpio_set_dir_in_masked(ENC_ALL_MASK);
    gpio_pull_up(ENC_LEFT);
    gpio_pull_up(ENC_RIGHT);
    gpio_pull_up(ENC_BTN);
    gpio_set_irq_enabled_with_callback(ENC_RIGHT, GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE, true, &enc_irq);
    gpio_set_irq_enabled_with_callback(ENC_LEFT, GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE, true, &enc_irq);
    gpio_set_irq_enabled_with_callback(ENC_BTN, GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE, true, &enc_irq);
}
#endif  // ENC_USE_STATE_MACHINE

#ifdef ENC_USE_PIO
static inline void quadrature_encoder_program_init(PIO pio, uint sm, uint pin, int max_step_rate) {
    pio_sm_set_consecutive_pindirs(pio, sm, pin, 2, false);
    gpio_pull_up(pin);
    gpio_pull_up(pin + 1);

    pio_sm_config c = quadrature_encoder_program_get_default_config(0);

    sm_config_set_in_pins(&c, pin);  // for WAIT, IN
    sm_config_set_jmp_pin(&c, pin);  // for JMP
    // shift to left, autopull disabled
    sm_config_set_in_shift(&c, false, false, 32);
    // don't join FIFO's
    sm_config_set_fifo_join(&c, PIO_FIFO_JOIN_NONE);

    // passing "0" as the sample frequency,
    if (max_step_rate == 0) {
        sm_config_set_clkdiv(&c, 1.0);
    } else {
        // one state machine loop takes at most 10 cycles
        float div = (float)clock_get_hz(clk_sys) / (10 * max_step_rate);
        sm_config_set_clkdiv(&c, div);
    }

    pio_sm_init(pio, sm, 0, &c);
    pio_sm_set_enabled(pio, sm, true);
}

static inline int32_t quadrature_encoder_get_count(PIO pio, uint sm) {
    uint ret;
    int n;

    // if the FIFO has N entries, we fetch them to drain the FIFO,
    // plus one entry which will be guaranteed to not be stale
    n = pio_sm_get_rx_fifo_level(pio, sm) + 1;
    while (n > 0) {
        ret = pio_sm_get_blocking(pio, sm);
        n--;
    }
    return ret;
}
void enc_irq(uint gpio, uint32_t events);
inline void setup_enc() {
    pio_add_program(pio, &quadrature_encoder_program);
    quadrature_encoder_program_init(pio, ENC_SM_ADDR, ENC_PIN_AB, 0);
    gpio_init(ENC_BTN);
    gpio_set_dir(ENC_BTN, GPIO_IN);
    gpio_pull_up(ENC_BTN);
    gpio_set_irq_enabled_with_callback(ENC_BTN, GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE, true, &enc_irq);

}

int8_t get_enc_pos_diff() {
    new_value = quadrature_encoder_get_count(pio, ENC_SM_ADDR);
    delta = new_value - old_value;
    if (delta / 4 > 0) {
        old_value = new_value;
        return 1;
    } else if (delta / 4 < 0) {
        old_value = new_value;
        return -1;
    }
    return 0;
}
#endif  // ENC_USE_PIO

bool get_enc_btn_state() {
    return enc_btn_state;
}

void enc_irq(uint gpio, uint32_t events) {
    enc_btn_state = (events & GPIO_IRQ_EDGE_FALL) ?: false;
}