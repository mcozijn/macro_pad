#include <pico/stdlib.h>
#include <hardware/i2c.h>
#include <pico/binary_info.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "display.h"
#include "font.h"

inline static void swap(int32_t *a, int32_t *b) {
    int32_t t = *a;
    *a = *b;
    *b = t;
}

inline static void fancy_write(i2c_inst_t *i2c, uint8_t addr, const uint8_t *src, size_t len, const char *name) {
    switch (i2c_write_blocking(i2c, addr, src, len, false)) {
        case PICO_ERROR_GENERIC:
            printf("[%s] addr not acknowledged!\n", name);
            break;
        case PICO_ERROR_TIMEOUT:
            printf("[%s] timeout!\n", name);
            break;
        default:
            break;
    }
}

inline static void display_write(display_t *p, uint8_t val) {
    uint8_t d[2] = {0x00, val};
    fancy_write(p->i2c_i, p->address, d, 2, "display_write");
}

bool display_init(display_t *p, uint16_t width, uint16_t height, uint8_t address, i2c_inst_t *i2c_instance) {
    p->width = (uint8_t)width;
    p->height = (uint8_t)height;
    p->pages = (uint8_t)(height / 8);
    p->address = address;
    p->i2c_i = i2c_instance;
    p->bufsize = p->pages * p->width;
    p->buffer = malloc(p->bufsize + 1);
    if (p->buffer == NULL) {
        p->bufsize = 0;
        return false;
    }
    ++(p->buffer);

    uint8_t cmds[] = {
        SET_DISP, SET_DISP_CLK_DIV, 0x80,
        SET_MUX_RATIO, (uint8_t)(height - 1),
        SET_DISP_OFFSET, 0x00, SET_DISP_START_LINE,
        SET_CHARGE_PUMP, p->external_vcc ? 0x10 : 0x14,
        SET_SEG_REMAP | 0x01, SET_COM_OUT_DIR | 0x08,
        SET_COM_PIN_CFG, width > 2 * height ? 0x02 : 0x12,
        SET_CONTRAST, 0xFF, SET_PRECHARGE, p->external_vcc ? 0x22 : 0xF1,
        SET_VCOM_DESEL, 0x30, SET_ENTIRE_ON, SET_NORM_INV,
        SET_DISP | 0x01, SET_MEM_ADDR, 0x00
    };

    for (size_t i = 0; i < sizeof(cmds); ++i) {
        display_write(p, cmds[i]);
    }
    return true;
}

inline void display_deinit(display_t *p) {
    free(p->buffer - 1);
}

inline void display_poweroff(display_t *p) {
    display_write(p, SET_DISP | 0x00);
}

inline void display_poweron(display_t *p) {
    display_write(p, SET_DISP | 0x01);
}

inline void display_contrast(display_t *p, uint8_t val) {
    display_write(p, SET_CONTRAST);
    display_write(p, val);
}

inline void display_invert(display_t *p, uint8_t inv) {
    display_write(p, SET_NORM_INV | (inv & 1));
}

inline void display_clear(display_t *p) {
    memset(p->buffer, 0, p->bufsize);
}

void display_clear_pixel(display_t *p, int32_t x, int32_t y) {
    if (x < 0 || y < 0 || x >= p->width || y >= p->height) return;

    uint8_t mask = (uint8_t)~(uint8_t)(0x1 << (y & 0x07));
    p->buffer[x + p->width * (y >> 3)] &= mask;
}

void display_draw_pixel(display_t *p, int32_t x, int32_t y) {
    if (x < 0 || y < 0 || x >= p->width || y >= p->height) return;
    p->buffer[x + p->width * (y >> 3)] |= (uint8_t)(0x1 << (y & 0x07));
}

void display_draw_line(display_t *p, int32_t x1, int32_t y1, int32_t x2, int32_t y2) {
    if (x1 > x2) {
        swap(&x1, &x2);
        swap(&y1, &y2);
    }

    if (x1 == x2) {
        if (y1 > y2) swap(&y1, &y2);
        for (int32_t i = y1; i <= y2; ++i)
            display_draw_pixel(p, x1, i);
        return;
    }

    float m = (float)(y2 - y1) / (float)(x2 - x1);
    for (int32_t i = x1; i <= x2; ++i) {
        float y = m * (float)(i - x1) + (float)y1;
        display_draw_pixel(p, i, (int32_t)y);
    }
}

void display_clear_square(display_t *p, int32_t x, int32_t y, int32_t width, int32_t height) {
    for (int32_t i = 0; i < width; ++i)
        for (int32_t j = 0; j < height; ++j)
            display_clear_pixel(p, x + i, y + j);
}

void display_draw_square(display_t *p, int32_t x, int32_t y, int32_t width, int32_t height) {
    for (int32_t i = 0; i < width; ++i)
        for (int32_t j = 0; j < height; ++j)
            display_draw_pixel(p, x + i, y + j);
}

void display_draw_empty_square(display_t *p, int32_t x, int32_t y, int32_t width, int32_t height) {
    display_draw_line(p, x, y, x + width, y);
    display_draw_line(p, x, y + height, x + width, y + height);
    display_draw_line(p, x, y, x, y + height);
    display_draw_line(p, x + width, y, x + width, y + height);
}

void display_draw_char_with_font(display_t *p, int32_t x, int32_t y, uint32_t scale, const uint8_t *font, char c) {
    if ((uint8_t)c < font[3] || (uint8_t)c > font[4]) return;

    uint32_t parts_per_line = ((uint32_t)(font[0] >> 3)) + (((font[0] & 7) > 0) ? 1U : 0U);
    for (uint8_t w = 0; w < font[1]; ++w) {
        int32_t pp = ((int32_t)((uint8_t)c - font[3])) * font[1] * (int32_t)parts_per_line + w * (int32_t)parts_per_line + 5;
        for (uint32_t lp = 0; lp < parts_per_line; ++lp) {
            uint8_t line = font[pp];
            for (int8_t j = 0; j < 8; ++j, line >>= 1) {
                if (line & 1) {
                    int32_t x_pos = x + ((int32_t)w * (int32_t)scale);
                    int32_t y_offset = (((int32_t)lp << 3) + (int32_t)j);
                    int32_t y_pos = y + y_offset * (int32_t)scale;
                    display_draw_square(p, x_pos, y_pos, (int32_t)scale, (int32_t)scale);
                }
            }
            ++pp;
        }
    }
}


void display_draw_string_with_font(display_t *p, int32_t x, int32_t y, uint32_t scale, const uint8_t *font, const char *s) {
    for (int32_t x_n = x; *s; x_n += (int32_t)((font[1] + font[2]) * scale)) {
        display_draw_char_with_font(p, x_n, y, scale, font, *(s++));
    }
}

void display_draw_char(display_t *p, int32_t x, int32_t y, uint32_t scale, char c) {
    display_draw_char_with_font(p, x, y, scale, font_8x5, c);
}

void display_draw_string(display_t *p, int32_t x, int32_t y, uint32_t scale, const char *s) {
    display_draw_string_with_font(p, x, y, scale, font_8x5, s);
}

static inline uint32_t display_bmp_get_val(const uint8_t *data, size_t offset, uint8_t size) {
    switch (size) {
        case 1:
            return data[offset];
        case 2:
            return data[offset] | (data[offset + 1] << 8);
        case 4:
            return data[offset] | (data[offset + 1] << 8) | (data[offset + 2] << 16) | (data[offset + 3] << 24);
        default:
            __builtin_unreachable();
    }
}

void display_bmp_show_image_with_offset(display_t *p, const uint8_t *data, long size, int32_t x_offset, int32_t y_offset) {
    if (size < 54) return;

    const uint32_t bfOffBits = display_bmp_get_val(data, 10, 4);
    const uint32_t biSize = display_bmp_get_val(data, 14, 4);
    const uint32_t biWidth = display_bmp_get_val(data, 18, 4);
    const int32_t biHeight = (int32_t)display_bmp_get_val(data, 22, 4);
    const uint16_t biBitCount = (uint16_t)display_bmp_get_val(data, 28, 2);
    const uint32_t biCompression = display_bmp_get_val(data, 30, 4);

    if (biBitCount != 1 || biCompression != 0) return;

    const int32_t table_start = 14 + (int32_t)biSize;
    uint8_t color_val = 0;

    for (uint8_t i = 0; i < 2; ++i) {
        if (!((data[table_start + i * 4] << 16) | (data[table_start + i * 4 + 1] << 8) | data[table_start + i * 4 + 2])) {
            color_val = i;
            break;
        }
    }

    uint32_t bytes_per_line = (biWidth / 8) + ((biWidth & 7) ? 1 : 0);
    if (bytes_per_line & 3) bytes_per_line = (bytes_per_line ^ (bytes_per_line & 3)) + 4;

    const uint8_t *img_data = data + bfOffBits;

    int32_t step = biHeight > 0 ? -1 : 1;
    int32_t border = biHeight > 0 ? -1 : -biHeight;

    for (int32_t y = biHeight > 0 ? biHeight - 1 : 0; y != border; y += step) {
        for (uint32_t x = 0; x < biWidth; ++x) {
            if (((img_data[x >> 3] >> (7 - (x & 7))) & 1) == color_val) {
                int32_t x_pos = x_offset + (int32_t)x;
                int32_t y_pos = y_offset + y;
                display_draw_pixel(p, x_pos, y_pos);
            }
        }
        img_data += bytes_per_line;
    }
}

inline void display_bmp_show_image(display_t *p, const uint8_t *data, long size) {
    display_bmp_show_image_with_offset(p, data, size, 0, 0);
}

void display_show(display_t *p) {
    uint8_t payload[] = {SET_COL_ADDR, 0, (uint8_t)(p->width - 1), SET_PAGE_ADDR, 0, (uint8_t)(p->pages - 1)};
    if (p->width == 64) {
        payload[1] += 32;
        payload[2] += 32;
    }

    for (size_t i = 0; i < sizeof(payload); ++i)
        display_write(p, payload[i]);

    *(p->buffer - 1) = 0x40;
    fancy_write(p->i2c_i, p->address, p->buffer - 1, p->bufsize + 1, "display_show");
}
