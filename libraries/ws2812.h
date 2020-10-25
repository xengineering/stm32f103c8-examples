

#ifndef WS2812_H
#define WS2812_H


#include <stdlib.h>
#include <libopencm3/stm32/gpio.h>


typedef struct ws2812_init_typedef{

	uint32_t gpio_port;
    uint16_t gpio_pin;
    uint32_t array_length;
    uint32_t matrix_width;
    uint8_t (*led_array)[3];

}ws2812_init_typedef;


void ws2812_init(ws2812_init_typedef *control_struct, uint32_t port, uint16_t pin, uint32_t array_length, uint32_t matrix_width);

void ws2812_send_reset(ws2812_init_typedef *control_struct);
void ws2812_send_zero(ws2812_init_typedef *control_struct);
void ws2812_send_one(ws2812_init_typedef *control_struct);

void ws2812_send_byte(ws2812_init_typedef *control_struct, uint8_t data);
void ws2812_send_led(ws2812_init_typedef *control_struct, uint8_t red, uint8_t green, uint8_t blue);
void ws2812_write_leds(ws2812_init_typedef *control_struct);

void ws2812_set_array_led(ws2812_init_typedef *control_struct, uint32_t index, uint8_t red, uint8_t green, uint8_t blue);
void ws2812_set_matrix_led(ws2812_init_typedef *control_struct, uint32_t x, uint32_t y, uint8_t red, uint8_t green, uint8_t blue);

void ws2812_clear_buffer(ws2812_init_typedef *control_struct);


#endif  /* WS2812_H */
