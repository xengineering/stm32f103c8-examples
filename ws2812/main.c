

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

#include "ws2812.h"


#define DELAY 30000  // 18,000,000 nops are one second
#define BRIGHTNESS 100


void clock_init(void);
void gpio_init(void);
void delay(void);


ws2812_init_typedef ws2812;


int main(void)
{
	clock_init();
	gpio_init();
	ws2812_init(&ws2812, GPIOB, GPIO13, 1, 1);

	while(1){
		/*ws2812_send_led(&ws2812, BRIGHTNESS, 0, 0);
		delay();
		ws2812_send_led(&ws2812, 0, BRIGHTNESS, 0);
		delay();
		ws2812_send_led(&ws2812, 0, 0, BRIGHTNESS);
		delay();
		ws2812_send_led(&ws2812, BRIGHTNESS, BRIGHTNESS, BRIGHTNESS);
		delay();*/

		for (uint16_t i=0; i<=255; i++){
			ws2812_send_led(&ws2812, i, 0, 0);
			delay();
		}
		for (uint16_t i=255; i>0; i--){
			ws2812_send_led(&ws2812, i, 0, 0);
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
