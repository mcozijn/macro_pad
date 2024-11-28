/** 
* @file display.h
* 
* Simple driver for SSD1306 displays
*/

#ifndef _inc_display
#define _inc_display

#include <pico/stdlib.h>
#include <hardware/i2c.h>

/**
 * @brief Defines commands used in SSD1306
 */
typedef enum {
    SET_CONTRAST       = 0x81,
    SET_ENTIRE_ON      = 0xA4,
    SET_NORM_INV       = 0xA6,
    SET_DISP           = 0xAE,
    SET_MEM_ADDR       = 0x20,
    SET_COL_ADDR       = 0x21,
    SET_PAGE_ADDR      = 0x22,
    SET_DISP_START_LINE= 0x40,
    SET_SEG_REMAP      = 0xA0,
    SET_MUX_RATIO      = 0xA8,
    SET_COM_OUT_DIR    = 0xC0,
    SET_DISP_OFFSET    = 0xD3,
    SET_COM_PIN_CFG    = 0xDA,
    SET_DISP_CLK_DIV   = 0xD5,
    SET_PRECHARGE      = 0xD9,
    SET_VCOM_DESEL     = 0xDB,
    SET_CHARGE_PUMP    = 0x8D
} display_command_t;

/**
 * @brief Holds the configuration
 */
typedef struct {
    uint8_t width;         /**< Width of display */
    uint8_t height;        /**< Height of display */
    uint8_t pages;         /**< Number of pages of display (calculated on initialization) */
    uint8_t address;       /**< I2C address of display */
    i2c_inst_t *i2c_i;     /**< I2C connection instance */
    bool external_vcc;     /**< Whether display uses external VCC */ 
    uint8_t *buffer;       /**< Display buffer */
    size_t bufsize;        /**< Buffer size */
} display_t;

bool display_init(display_t *p, uint16_t width, uint16_t height, uint8_t address, i2c_inst_t *i2c_instance);
void display_deinit(display_t *p);
void display_poweroff(display_t *p);
void display_poweron(display_t *p);
void display_contrast(display_t *p, uint8_t val);
void display_invert(display_t *p, uint8_t inv);
void display_show(display_t *p);
void display_clear(display_t *p);
void display_clear_pixel(display_t *p, int32_t x, int32_t y);
void display_draw_pixel(display_t *p, int32_t x, int32_t y);
void display_draw_line(display_t *p, int32_t x1, int32_t y1, int32_t x2, int32_t y2);
void display_clear_square(display_t *p, int32_t x, int32_t y, int32_t width, int32_t height);
void display_draw_square(display_t *p, int32_t x, int32_t y, int32_t width, int32_t height);
void display_draw_empty_square(display_t *p, int32_t x, int32_t y, int32_t width, int32_t height);
void display_bmp_show_image_with_offset(display_t *p, const uint8_t *data, long size, int32_t x_offset, int32_t y_offset);
void display_bmp_show_image(display_t *p, const uint8_t *data, long size);
void display_draw_char_with_font(display_t *p, int32_t x, int32_t y, uint32_t scale, const uint8_t *font, char c);
void display_draw_char(display_t *p, int32_t x, int32_t y, uint32_t scale, char c);
void display_draw_string_with_font(display_t *p, int32_t x, int32_t y, uint32_t scale, const uint8_t *font, const char *s);
void display_draw_string(display_t *p, int32_t x, int32_t y, uint32_t scale, const char *s);

#endif
