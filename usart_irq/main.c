

#include <errno.h>
#include <stdio.h>
#include <unistd.h>

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

#include "usart.h"


#define DELAY 1000000
#define RX_BUFFER_SIZE 50
#define TX_BUFFER_SIZE 50


void clock_init(void);
void gpio_init(void);
void delay(void);
int _write(int file, char *ptr, int len);


USART usart3;


int main(void)
{
	uint8_t rx_buffer[RX_BUFFER_SIZE];
	uint8_t tx_buffer[TX_BUFFER_SIZE];

	clock_init();
	gpio_init();
	usart_init(&usart3, rx_buffer, RX_BUFFER_SIZE, tx_buffer, TX_BUFFER_SIZE);

	gpio_set(GPIOC, GPIO13);

	char welcome[] = "Hello world!\r";
	usart_write(&usart3, welcome);

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
}


void delay(void)
{
	for (int i = 0; i < DELAY; i++) {
		__asm__("nop");
	}
}


int _write(int file, char *ptr, int len)
{
	int i;

	if (file == STDOUT_FILENO || file == STDERR_FILENO) {
		for (i = 0; i < len; i++) {
			if (ptr[i] == '\n') {
				uint8_t cr = (uint8_t)'\r';
				usart_write_bytes(&usart3, &cr, 1);
			}
			usart_write_bytes(&usart3, (uint8_t *)&ptr[i], 1);
		}
		return i;
	}
	errno = EIO;
	return -1;
}
