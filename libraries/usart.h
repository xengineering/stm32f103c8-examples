

#ifndef USART_H
#define USART_H


#include <string.h>

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/cm3/nvic.h>

#include "fifo.h"

#define USART_BUFFER_SIZE 30


typedef struct USART{

    FIFO rx_fifo;
    FIFO tx_fifo;

} USART;


// init functions

void usart_init(USART *usart);
void usart_deinit(USART *usart);


// write functions

uint16_t usart_write_bytes(USART *usart, uint8_t *data_source, uint16_t data_length);
uint16_t usart_write(USART *usart, char *str);
//uint16_t usart_writeln(USART *usart, char *str);


// read functions

uint16_t usart_read_bytes(USART *usart, uint8_t *data_sink, uint16_t data_length);
//uint16_t usart_read(USART *usart, char *data_sink, uint16_t data_length);
//uint16_t usart_readln(USART *usart, char *data_sink);


#endif /* USART_H */
