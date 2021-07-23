#ifndef __SHARP_MEM_H
#define __SHARP_MEM_H

/**************************************************************************
    Sharp Memory Display Connector
    -----------------------------------------------------------------------
    Pin   Function        Notes
    ===   ==============  ===============================
      1   VIN             3.3-5.0V (into LDO supply)
      2   3V3             3.3V out
      3   GND
      4   SCLK            Serial Clock
      5   MOSI            Serial Data Input
      6   CS              Serial Chip Select
      9   EXTMODE         COM Inversion Select (Low = SW clock/serial)
      7   EXTCOMIN        (using SW mode)
      8   DISP            Display On(High)/Off(Low)
 **************************************************************************/

// SPI1 is on GPIOA
#define LCD_SPI SPI1
#define LCD_CS GPIO4
#define LCD_SCK GPIO5
#define LCD_MOSI GPIO7

#define LCD_WIDTH 400
#define LCD_HEIGHT 240
#define FRAME_SIZE (LCD_WIDTH * LCD_HEIGHT)
#define FRAME_SIZE_BYTES (FRAME_SIZE / 8)

void sharpmem_init(void);
void sharpmem_clear(void);
void sharpmem_refresh(void);

void sharpmem_write_test_pattern(void);

#endif
