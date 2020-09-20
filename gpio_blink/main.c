

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>


#define DELAY 1000000


void clock_init(void);
void gpio_init(void);
void delay(void);


int main(void)
{
	clock_init();
	gpio_init();

	while(1){
		gpio_toggle(GPIOC, GPIO13);
		delay();
	}

	return 0;
}


void clock_init(void)
{
	rcc_periph_clock_enable(RCC_GPIOC);  // for PC13 blinking
}


void gpio_init(void)
{
	gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO13);
	gpio_set(GPIOC, GPIO13);
}


void delay(void)
{
	for (int i = 0; i < DELAY; i++) {
		__asm__("nop");
	}
}
