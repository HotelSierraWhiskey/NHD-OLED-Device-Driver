# NHD-1.27-12896ASC3 Device Driver

This is the SPI bitbang driver code for the Newhaven Display 1.27" OLED.
I've used this code in PIC18F-based projects. The `set_X_PIN` macros have been left untouched - change the pins as required. 
This driver requires a non-blocking delay routine. 

The datasheet for this OLED device can be found [here](https://www.newhavendisplay.com/specs/NHD-1.27-12896ASC3.pdf)
