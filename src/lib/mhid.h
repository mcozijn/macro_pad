#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bsp/board_api.h"
#include "tusb.h"
#include "usb_descriptors.h"

/* Blink pattern
 * - 250 ms  : device not mounted
 * - 1000 ms : device mounted
 * - 2500 ms : device is suspended
 */
enum {
   BLINK_NOT_MOUNTED = 250,
   BLINK_MOUNTED = 1000,
   BLINK_SUSPENDED = 2500,
};
typedef int8_t (*get_key_fn)(void);
typedef int8_t (*get_enc_fn)(void);
typedef void (*set_dpy_fn)(int8_t keycode);

void hid_task(get_key_fn get_key, get_enc_fn get_enc, set_dpy_fn set_dpy);