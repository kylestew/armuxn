#include <libopencm3/stm32/gpio.h>

#include "clock.h"
#include "sharp-mem.h"

int main(void) {
    clock_setup();

    sharpmem_init();
    sharpmem_clear();

    msleep(200);

    sharpmem_write_test_pattern();

    while (1) {
        sharpmem_refresh();

        // note: need to refresh display even if no update at
        // least f=0.5 so that VCOM can be toggled
        msleep(500);
    }
}
