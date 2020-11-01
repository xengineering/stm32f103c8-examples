

#include <errno.h>
#include <stdio.h>
#include <unistd.h>

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

#include "usart.h"


#define DELAY 1000000
#define RX_BUFFER_SIZE 1000
#define TX_BUFFER_SIZE 1000


void clock_init(void);
void gpio_init(void);
void delay(void);


USART usart3;


int main(void)
{
	// setup
	clock_init();
	gpio_init();
	uint8_t rx_buffer[RX_BUFFER_SIZE];
	uint8_t tx_buffer[TX_BUFFER_SIZE];
	usart_init(&usart3, rx_buffer, RX_BUFFER_SIZE, tx_buffer, TX_BUFFER_SIZE);

	// welcome message
	usart_writeln(&usart3, "This is a demo of USART via IRQ. Just type something!");

	// return any input
	uint8_t buff;
	while(1){
		if (usart_read_bytes(&usart3, &buff, 1)){
			usart_write_bytes(&usart3, &buff, 1);
		}
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
	gpio_clear(GPIOC, GPIO13);
	gpio_set(GPIOC, GPIO13);
}


void delay(void)
{
	for (int i = 0; i < DELAY; i++) {
		__asm__("nop");
	}
}
