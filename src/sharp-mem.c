#include "sharp-mem.h"

#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/spi.h>
#include <stdlib.h>
#include <string.h>

#include "clock.h"

#define SHARPMEM_BIT_WRITECMD (0x01)   // 0x80 LSB
#define SHARPMEM_BIT_CLEAR (0x04)      // 0x20 LSB
#define SHARPMEM_BIT_VCOM_HIGH (0x02)  // 0x40 LSB
#define SHARPMEM_BIT_VCOM_LOW (0x0)

static uint8_t* sharpmem_buffer;
static uint8_t sharpmem_vcom;

#define CHIP_SELECT gpio_set(GPIOA, GPIO4);
#define CHIP_DESELECT gpio_clear(GPIOA, GPIO4);
/*
   VCOM explanation:
   The display needs a signal at frequency 0.5-20Hz to flip a common bias so no DC-bias forms.
   This can be done in hardware, but the Adafruit engineers wired up the display for software
   mode so no external clock is needed. This does mean that we need to constantly be toggling
   this setting at least every 500ms.

   Conveniently Sharp lets us send the toggle command along with other commands. It's pretty
   simple to bitwise OR along with other commands and only takes up 1 bit (1s place, LSB).
   */
#define TOGGLE_VCOM                                                                     \
    do {                                                                                \
        sharpmem_vcom = sharpmem_vcom ? SHARPMEM_BIT_VCOM_LOW : SHARPMEM_BIT_VCOM_HIGH; \
    } while (0);

void sharpmem_init(void) {
    // setup GPIO lines for SPI port and control lines for display
    rcc_periph_clock_enable(RCC_GPIOA);

    // CS
    gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LCD_CS);
    CHIP_DESELECT;

    // SCK / MOSI pins
    gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, LCD_SCK | LCD_MOSI);
    gpio_set_af(GPIOA, GPIO_AF5, LCD_SCK | LCD_MOSI);

    // enable SPI1
    rcc_periph_clock_enable(RCC_SPI1);
    spi_reset(LCD_SPI);
    spi_init_master(LCD_SPI, SPI_CR1_BAUDRATE_FPCLK_DIV_8, SPI_CR1_CPOL_CLK_TO_0_WHEN_IDLE,
                    SPI_CR1_CPHA_CLK_TRANSITION_1, SPI_CR1_DFF_8BIT, SPI_CR1_LSBFIRST);
    spi_enable_software_slave_management(LCD_SPI);
    spi_set_nss_high(LCD_SPI);

    spi_enable(LCD_SPI);

    // put VCOM in known state
    sharpmem_vcom = SHARPMEM_BIT_VCOM_HIGH;

    // set up the display memory
    sharpmem_buffer = (uint8_t*)malloc(FRAME_SIZE_BYTES);
}

void sharpmem_clear(void) {
    memset(sharpmem_buffer, 0xff, FRAME_SIZE_BYTES);

    CHIP_SELECT;

    // send the clear screen command instead of a full refresh (quicker)
    // includes the VCOM command
    spi_xfer(LCD_SPI, sharpmem_vcom | SHARPMEM_BIT_CLEAR);
    spi_xfer(LCD_SPI, 0x00);

    TOGGLE_VCOM;

    CHIP_DESELECT;
}

void sharpmem_refresh(void) {
    CHIP_SELECT;

    spi_xfer(LCD_SPI, (uint8_t)sharpmem_vcom | SHARPMEM_BIT_WRITECMD);
    TOGGLE_VCOM;

    uint16_t i, j, current_line;
    uint8_t bytes_per_line = LCD_WIDTH / 8;
    uint16_t total_bytes = FRAME_SIZE_BYTES;
    for (i = 0; i < total_bytes; i += bytes_per_line) {
        // address
        current_line = ((i + 1) / (LCD_WIDTH / 8)) + 1;
        spi_xfer(LCD_SPI, (uint8_t)current_line);

        // pixel data
        for (j = 0; j < bytes_per_line; j++) {
            spi_xfer(LCD_SPI, (uint8_t)sharpmem_buffer[i + j]);
        }

        // end of line trailer
        spi_xfer(LCD_SPI, (uint8_t)0x00);
    }

    // another trailing 8 bits for a total of 16 bits to end transmission
    spi_xfer(LCD_SPI, (uint8_t)0x00);

    CHIP_DESELECT;
}

void sharpmem_write_test_pattern(void) {
    for (int i = 0; i < FRAME_SIZE_BYTES; i++) {
        sharpmem_buffer[i] = i % 3 ? i % 2 ? 0XFE : 0xFF : 0XAF;
    }
}

