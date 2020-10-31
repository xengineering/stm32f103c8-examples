

#ifndef WS2812_H
#define WS2812_H


#include <stdlib.h>
#include <libopencm3/stm32/gpio.h>

#define WS2812_NUMBER_OF_COLORS 3


typedef struct WS2812_ARRAY{

	uint32_t gpio_port;
    uint16_t gpio_pin;
    uint8_t *array_buffer;
    uint32_t array_length;

}WS2812_ARRAY;


void ws2812_init(WS2812_ARRAY *control_struct, uint32_t port, uint16_t pin, uint8_t *buffer_ptr, uint32_t array_length);

void ws2812_send_reset(WS2812_ARRAY *control_struct);
void ws2812_send_zero(WS2812_ARRAY *control_struct);
void ws2812_send_one(WS2812_ARRAY *control_struct);

void ws2812_send_byte(WS2812_ARRAY *control_struct, uint8_t data);
void ws2812_send_led(WS2812_ARRAY *control_struct, uint8_t red, uint8_t green, uint8_t blue);

void ws2812_write_leds(WS2812_ARRAY *control_struct);
void ws2812_set_array_led(WS2812_ARRAY *control_struct, uint32_t index, uint8_t red, uint8_t green, uint8_t blue);
void ws2812_clear_buffer(WS2812_ARRAY *control_struct);


#endif  /* WS2812_H */
