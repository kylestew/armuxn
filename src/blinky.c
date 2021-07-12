#include <libopencm3/cm3/systick.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>

static void systick_setup(void) {
    systick_set_clocksource(STK_CSR_CLKSOURCE_AHB);

    STK_CVR = 0;

    // to trigger an interrupt every millisecond, set reload
    // value to be the speed of the processor 1000 - 1
    systick_set_reload(rcc_ahb_frequency / 1000 - 1);

    // enable interrupts from the system tick clock
    systick_interrupt_enable();

    // enable systick counter
    systick_counter_enable();
}

static volatile uint64_t _millis = 0;
uint64_t millis(void) { return _millis; }
void sys_tick_handler(void) { _millis++; }

void delay(uint64_t duration) {
    const uint64_t until = millis() + duration;
    while (millis() < until)
        ;
}

static void gpio_setup(void) {
    rcc_periph_clock_enable(RCC_GPIOD);

    // LEDs arranged clock wise
    // GREEN LED - portD pin 12
    gpio_mode_setup(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO12);
    // ORANGE LED - portD pin 13
    gpio_mode_setup(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO13);
    // RED LED - portD pin 14
    gpio_mode_setup(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO14);
    // BLUE LED - portD pin 15
    gpio_mode_setup(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO15);
}

int main(void) {
    systick_setup();
    gpio_setup();

    int rest_period = 1000.0 / 10;
    while (1) {
        gpio_set(GPIOD, GPIO12);  // G
        delay(rest_period);
        delay(rest_period);
        delay(rest_period);

        gpio_set(GPIOD, GPIO13);  // O
        delay(rest_period);

        gpio_clear(GPIOD, GPIO12);  // G
        gpio_set(GPIOD, GPIO14);    // R
        delay(rest_period);

        gpio_clear(GPIOD, GPIO13);  // O
        gpio_set(GPIOD, GPIO15);    // B
        delay(rest_period);

        gpio_clear(GPIOD, GPIO14);  // R
        gpio_set(GPIOD, GPIO12);    // G
        delay(rest_period);

        gpio_clear(GPIOD, GPIO15);  // B
        delay(rest_period);

        delay(rest_period);
        delay(rest_period);
    }

    return 0;
}
