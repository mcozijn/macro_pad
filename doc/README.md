# A list of required functions
Split by components

## Screen
- `bool ssd1306_init(ssd1306_t *p, uint16_t width, uint16_t height, uint8_t address, i2c_inst_t *i2c_instance);` - Initialize the SSD1306 display
- `void ssd1306_draw_string(ssd1306_t *p, int32_t x, int32_t y, uint32_t scale, const char *s);`
- `void ssd1306_draw_pixel(ssd1306_t *p, int32_t x, int32_t y);`
- `void ssd1306_draw_char(ssd1306_t *p, int32_t x, int32_t y, uint32_t scale, char c);`
- `void ssd1306_bmp_show_image(ssd1306_t *p, const uint8_t *data, long size);`
- `void ssd1306_clear(ssd1306_t *p);`


## Encoder
idfk lol

## TinyUSB
- The entirety of `/pico-sdk/lib/tinyusb/src/class/hid/hid.h`
- `tud_task();` - Call this in the main loop to handle USB events
- `hid_task();` - Call this in the main loop to handle HID events
- `static void send_hid_report(uint8_t report_id, int32_t btn)` - Send a HID report


# Material that should be covered (software only)
- How to set up the Pico SDK
- Which libraries are we using, CMake, etc.
- Initializing pins and laying out the foundation for the project
- Setting up
  - SSD1306
  - Encoder
  - TinyUSB
- Drawing text and images on the screen
- Reading the encoder
- Sending HID reports
- Sending keystrokes (arrays of keys) and strings
- Mention more advanced features like the ability to send mouse and joystick events lol