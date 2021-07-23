#include "clock.h"

#include <libopencm3/cm3/systick.h>
#include <libopencm3/stm32/rcc.h>

// millis since program startup
static volatile uint32_t system_millis = 0;

void sys_tick_handler(void) { system_millis++; }

void msleep(uint32_t delay) {
    uint32_t wake = system_millis + delay;
    while (wake > system_millis)
        ;
}

uint32_t mtime(void) { return system_millis; }

void clock_setup(void) {
    rcc_clock_setup_pll(&rcc_hse_8mhz_3v3[RCC_CLOCK_3V3_168MHZ]);

    // clock rate / 1000 to get 1ms interrupt rate
    systick_set_reload(rcc_ahb_frequency / 1000);
    systick_set_clocksource(STK_CSR_CLKSOURCE_AHB);
    systick_counter_enable();

    // enable interrupts from the system tick clock
    // (do this last)
    systick_interrupt_enable();
}
