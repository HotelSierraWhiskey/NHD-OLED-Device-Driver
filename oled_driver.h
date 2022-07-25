#ifndef _OLED_DRIVER_H_
#define _OLED_DRIVER_H_

#include "os/system_time.h"

//  NHD-1.27-12896ASC3 OLED display dimension macros
#define MAX_DISPLAY_HEIGHT 0x60 + 1
#define MAX_DISPLAY_WIDTH 0x7F


typedef uint32_t color_t;


extern void oled_init(void);

extern void oled_tx_command(uint8_t command);

extern void oled_tx_data(uint8_t data);

extern void oled_set_column_address(uint8_t x_start, uint8_t x_end);

extern void oled_set_row_address(uint8_t y_start, uint8_t y_end);

extern void oled_reposition_cursor(uint8_t x, uint8_t y);

extern void oled_write_rgb_pixel(color_t color);

extern void oled_write_rgb_pixel_at(uint8_t x, uint8_t y, color_t color);

extern void oled_fill_display(color_t color);

extern void oled_write_memory_start(void);


#endif


