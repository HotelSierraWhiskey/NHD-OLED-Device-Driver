#include "pic_header.h"
#include "pins.h"
#include "oled_driver.h"


/* 
    Device driver code for the NHD12712896ASC3 OLED display.
    Datasheet can be found here:
        https://www.newhavendisplay.com/specs/NHD-1.27-12896ASC3.pdf
 */


#define set_STROBE_PIN(state) LATDbits.LATD0 = state 
#define set_CLOCK_PIN(state) LATAbits.LATA6 = state
#define set_DATA_PIN(state) LATCbits.LATC2 = state 
#define set_DC_PIN(state) LATCbits.LATC0 = state
#define set_CS_PIN(state) LATDbits.LATD0 = state
#define set_RESET_PIN(state) LATAbits.LATA7 = state

#define COMMAND 0
#define DATA 1

#define TX_DELAY 2
#define RESET_DELAY 500


void spi_bitbang_tx_word(uint8_t word) {

    set_STROBE_PIN(0);

    uint8_t mask = 0x80;

    for (uint8_t i = 0; i < 8; i++) {

        set_CLOCK_PIN(0);

        if ((word & mask) >> 7 == 1) {
            set_DATA_PIN(1);
        } 
        else {
            set_DATA_PIN(0);
        }

        set_CLOCK_PIN(1);
        word = word << 1;
    }
    set_STROBE_PIN(1);
}


void oled_tx_command(uint8_t command) {
    set_DC_PIN(COMMAND);
    spi_bitbang_tx_word(command);
}


void oled_tx_data(uint8_t data) {
    set_DC_PIN(DATA);
    spi_bitbang_tx_word(data);
}


void oled_set_column_address(uint8_t x_start, uint8_t x_end) {
    oled_tx_command(0x15);  // command to set column address
    oled_tx_data(x_start);
    oled_tx_data(x_end);
}


void oled_set_row_address(uint8_t y_start, uint8_t y_end) {
    oled_tx_command(0x75);  // command to set row address
    oled_tx_data(y_start);
    oled_tx_data(y_end);
}


void oled_reposition_cursor(uint8_t x, uint8_t y) {
    oled_set_column_address(x, MAX_DISPLAY_WIDTH);
    oled_set_row_address(y, MAX_DISPLAY_HEIGHT);
}


void oled_write_rgb_pixel(color_t color) {
    oled_tx_data((color>>16));
    oled_tx_data((color>>8));
    oled_tx_data(color);
}


// not consistent with oled_write_rgb_pixel
void oled_write_rgb_pixel_at(uint8_t x, uint8_t y, color_t color) {
    oled_reposition_cursor(x, y);
    oled_write_memory_start();
    oled_write_rgb_pixel(color);
}


void oled_fill_display(color_t color) {
    oled_set_column_address(0, MAX_DISPLAY_WIDTH);
    oled_set_row_address(0, MAX_DISPLAY_HEIGHT);
    oled_write_memory_start();
    for (uint8_t i = 0; i < MAX_DISPLAY_HEIGHT; i++) {
        for (uint8_t j = 0; j < MAX_DISPLAY_WIDTH; j++) {
            oled_write_rgb_pixel(color);
        }
    }
}


void oled_write_memory_start(void) {
    oled_tx_command(0x5C);
}


void oled_init(void) {
    set_RESET_PIN(0);
    delay_ms(RESET_DELAY);
    set_RESET_PIN(1);
    delay_ms(RESET_DELAY);

    set_CS_PIN(0);          // chip select pin, tied to the OLEDCS pin on oled device (active low)

    oled_tx_command(0xFD);  // command lock setting
    oled_tx_data(0x12);     // unlock OLED driver mcu interface from entering command               
    oled_tx_command(0xFD);  // command lock setting 
    oled_tx_data(0xB1);     // unlock commands A2, B1, B3, BB, BE, C1

    oled_tx_command(0xAE);  // sleep mode (display off)
    oled_tx_command(0xB3);  // unlock OLED driver mcu interface from entering command

    oled_tx_command(0xB3);  // clock & freq
    oled_tx_data(0xF1);     // clock=Diviser+1 frequency=fh 
    
    oled_tx_command(0xCA);  // Duty
    oled_tx_data(0x7F);     // OLED _END+1 

    oled_tx_command(0xA2);  //Display offset 
    oled_tx_data(0x00); 
    
    oled_tx_command(0xA1);  //Set display start line 
    oled_tx_data(0x00);     //0x00 start line 
    
    oled_tx_command(0xA0);  //Set Re-map, color depth 
    oled_tx_data(0xA0);     //8-bit 262K 
    
    oled_tx_data(0xC7);     //Set master contrast 
    oled_tx_command(0x0F);  //

    oled_tx_command(0xB2);  //display enhancement 
    oled_tx_data(0xA4);   
    oled_tx_data(0x00); 
    oled_tx_data(0x00);

    oled_tx_command(0xA6);  //normal display (?)

    oled_tx_command(0xAF);  // display on

}