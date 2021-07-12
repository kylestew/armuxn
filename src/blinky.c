#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>

static void gpio_setup(void) {
    /* Enable GPIOD clock. */
    /* Manually: */
    // RCC_AHB1ENR |= RCC_AHB1ENR_IOPDEN;
    /* Using API functions: */
    rcc_periph_clock_enable(RCC_GPIOD);

    /* Set GPIO12 (in GPIO port D) to 'output push-pull'. */
    /* Manually: */
    // GPIOD_CRH = (GPIO_CNF_OUTPUT_PUSHPULL << (((8 - 8) * 4) + 2));
    // GPIOD_CRH |= (GPIO_MODE_OUTPUT_2_MHZ << ((8 - 8) * 4));
    /* Using API functions: */
    gpio_mode_setup(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO12);
}

int main(void) {
    int i;

    gpio_setup();

    // Since our LED is on GPIO bank A, we need to enable
    // the peripheral clock to this GPIO bank in order to use it.
    // rcc_periph_clock_enable(RCC_GPIOA);

    // Our test LED is connected to Port A pin 11, so let's set it as output
    // gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO11);

    // Now, let's forever toggle this LED back and forth
    while (1) {
        gpio_toggle(GPIOD, GPIO12);
        for (i = 0; i < 1000000; i++) {
            __asm__("nop");
        }
    }

    return 0;
}
