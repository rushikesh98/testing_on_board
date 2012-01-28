#include "rcc.h"
#include "gpio.h"

/* Set STM32 to 72 MHz. */
void clock_setup(void)
{
	rcc_clock_setup_in_hse_8mhz_out_72mhz();

	/* Enable GPIOC clock. */
	rcc_peripheral_enable_clock(&RCC_APB2ENR, RCC_APB2ENR_IOPCEN);
}

void gpio_setup(void)
{
	/* Set GPIO6/7 (in GPIO port C) to 'output push-pull'. */
	gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_50_MHZ,
		      GPIO_CNF_OUTPUT_PUSHPULL, GPIO6);
	gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_50_MHZ,
		      GPIO_CNF_OUTPUT_PUSHPULL, GPIO7);
}

int main(void)
{
	int i;

	clock_setup();
	gpio_setup();

	/* Blink the LEDs (PC6 and PC7) on the board. */
	while (1) {
		gpio_toggle(GPIOC, GPIO6);	/* STAT1 LED on/off */
		gpio_toggle(GPIOC, GPIO7);	/* STAT2 LED on/off */
		for (i = 0; i < 8000000; i++)	/* Wait a bit. */
			__asm__("nop");
	}

	return 0;
}
