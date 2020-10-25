

#include "usart.h"


extern USART usart3;


// init functions

void usart_init(USART *usart, uint8_t *rx_buffer, uint16_t rx_buffersize, uint8_t *tx_buffer, uint16_t tx_buffersize)
{
    // clock config
    rcc_periph_clock_enable(RCC_GPIOB);  // for USART3
    rcc_periph_clock_enable(RCC_USART3);  // for USART3

    // gpio config
    gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO_USART3_TX);  // TX setup
    gpio_set_mode(GPIOB, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT, GPIO_USART3_RX);  // RX setup

    // nvic config
    nvic_enable_irq(NVIC_USART3_IRQ);
    usart_enable_rx_interrupt(USART3);

    // usart config
    usart_set_baudrate(USART3, 9600);
    usart_set_databits(USART3, 8);
    usart_set_flow_control(USART3, USART_FLOWCONTROL_NONE);
    usart_set_mode(USART3, USART_MODE_TX_RX);
    usart_set_parity(USART3, USART_PARITY_NONE);
    usart_set_stopbits(USART3, USART_STOPBITS_1);
    usart_enable(USART3);

    // fifo init
    fifo_init(&usart->rx_fifo, rx_buffer, rx_buffersize);
    fifo_init(&usart->tx_fifo, tx_buffer, tx_buffersize);
}


// write functions

uint16_t usart_write_bytes(USART *usart, uint8_t *data_source, uint16_t data_length){
    if ((data_length + fifo_bytes_available(&usart->tx_fifo)) <= fifo_get_buffersize(&usart->tx_fifo)){
        for (int i=0; i<data_length; i++){
            fifo_put_byte(&usart->tx_fifo, &data_source[i]);
        }
        usart_enable_tx_interrupt(USART3);
        return data_length;
    }
    else{
        return 0;
    }
}

uint16_t usart_write(USART *usart, char *str)
{
    for (uint16_t i=0; i<(uint16_t)strlen(str); i++){
        usart_write_bytes(usart, (uint8_t *)&str[i], 1);
    }
    return (uint16_t)strlen(str);
}
//uint16_t usart_writeln(USART *usart, char *str);


// read functions

uint16_t usart_read_bytes(USART *usart, uint8_t *data_sink, uint16_t data_length)
{
    if (data_length <= fifo_bytes_available(&usart->rx_fifo)){
        for (int i=0; i<data_length; i++){
            fifo_get_byte(&usart->rx_fifo, &data_sink[i]);
        }
        return data_length;
    }
    else{
        return 0;
    }
}
//uint16_t usart_read(USART *usart, char *data_sink, uint16_t data_length);
//uint16_t usart_readln(USART *usart, char *data_sink);


// usart isr

void usart3_isr(void)
{

	/* Check if we were called because of RXNE. */
	if (((USART_CR1(USART3) & USART_CR1_RXNEIE) != 0) &&
		((USART_SR(USART3) & USART_SR_RXNE) != 0)) {

		/* Retrieve the data from the peripheral. */
		uint8_t data = usart_recv(USART3);
        fifo_put_byte(&usart3.rx_fifo, &data);

		/* Enable transmit interrupt so it sends back the data. */
		//USART_CR1(USART3) |= USART_CR1_TXEIE;
	}

	/* Check if we were called because of TXE. */
	if (((USART_CR1(USART3) & USART_CR1_TXEIE) != 0) &&
		((USART_SR(USART3) & USART_SR_TXE) != 0)) {

		/* Put data into the transmit register. */
        if (fifo_bytes_available(&usart3.tx_fifo)){
            uint8_t data;
            fifo_get_byte(&usart3.tx_fifo, &data);
            usart_send(USART3, data);
            if(data == '\r'){
                uint8_t nl = (uint8_t)'\n';
                usart_write_bytes(&usart3, &nl, 1);
            }
            if (fifo_bytes_available(&usart3.tx_fifo)){
                usart_enable_tx_interrupt(USART3);
            }
            else{
                usart_disable_tx_interrupt(USART3);
            }
        }
    }
}
