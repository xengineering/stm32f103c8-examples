

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

#include "ws2812.h"


#define DELAY 90000  // 18,000,000 nops are one second @ 72 MHz (one nop --> 4 clock cycles)
#define BRIGHTNESS 30
#define LED_ARRAY_LENGTH 3


void clock_init(void);
void gpio_init(void);
void delay(void);


int main(void)
{
	clock_init();
	gpio_init();
	uint8_t led_array_buffer[LED_ARRAY_LENGTH*WS2812_NUMBER_OF_COLORS];
	WS2812_ARRAY led_panel;
	ws2812_init(&led_panel, GPIOB, GPIO13, (uint8_t *)led_array_buffer, LED_ARRAY_LENGTH);

	while(1)
	{
		for (uint16_t i=0; i<=BRIGHTNESS; i++)
		{
			ws2812_set_array_led(&led_panel, 0, i, 0, 0);
			ws2812_set_array_led(&led_panel, 1, 0, i, 0);
			ws2812_set_array_led(&led_panel, 2, 0, 0, i);

			ws2812_write_leds(&led_panel);
			delay();
		}
		for (uint16_t i=BRIGHTNESS; i>0; i--)
		{
			ws2812_set_array_led(&led_panel, 0, i, 0, 0);
			ws2812_set_array_led(&led_panel, 1, 0, i, 0);
			ws2812_set_array_led(&led_panel, 2, 0, 0, i);

			ws2812_write_leds(&led_panel);
			delay();
		}
	}

	return 0;
}


void clock_init(void)
{
	rcc_clock_setup_in_hse_12mhz_out_72mhz();
	rcc_periph_clock_enable(RCC_GPIOB);  // for ws2812
	rcc_periph_clock_enable(RCC_GPIOC);  // for PC13 blinking
}


void gpio_init(void)
{
	// on board led blinking
	gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO13);
	gpio_set(GPIOC, GPIO13);

	// ws2812 output pin
	gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO13);
	gpio_set(GPIOB, GPIO13);
}


void delay(void)
{
	for (int i = 0; i < DELAY; i++) {
		__asm__("nop");
	}
}
