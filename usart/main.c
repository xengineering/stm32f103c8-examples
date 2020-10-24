

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>


#define DELAY 1000000


void clock_init(void);
void gpio_init(void);
void usart_init(void);
void delay(void);


int main(void)
{
	clock_init();
	gpio_init();
	usart_init();

	while(1){
		delay();
		usart_send_blocking(USART3, 'H');
		usart_send_blocking(USART3, 'e');
		usart_send_blocking(USART3, 'l');
		usart_send_blocking(USART3, 'l');
		usart_send_blocking(USART3, 'o');
		usart_send_blocking(USART3, '\r');
		usart_send_blocking(USART3, '\n');
		delay();
	}

	return 0;
}


void clock_init(void)
{
	rcc_periph_clock_enable(RCC_GPIOB);   // for USART3
    rcc_periph_clock_enable(RCC_USART3);  // for USART3
}


void gpio_init(void)
{
	gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO_USART3_TX);  // TX setup
	gpio_set_mode(GPIOB, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT, GPIO_USART3_RX);  // RX setup
}


void usart_init(void)
{
	usart_set_baudrate(USART3, 9600);
	usart_set_databits(USART3, 8);
	usart_set_flow_control(USART3, USART_FLOWCONTROL_NONE);
	usart_set_mode(USART3, USART_MODE_TX_RX);
	usart_set_parity(USART3, USART_PARITY_NONE);
	usart_set_stopbits(USART3, USART_STOPBITS_1);
	usart_enable(USART3);
}


void delay(void)
{
	for (int i = 0; i < DELAY; i++) {
		__asm__("nop");
	}
}
