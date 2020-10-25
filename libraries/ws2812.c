

/*

# WS2812 Library for libopencm3

Please mind these restrictions:

- Only usable with libopencm3
- Newer versions of libopencm3 may be not compatibel (libopencm3 api is not stable yet)
- Just usable with a clock frequency of 80 MHz
- You have to setup the clock and gpio yourself

*/


#include "ws2812.h"

#define NUMBER_NOPS_RESET 1080  // > 50 µs --> 60 µs (to be sure) = 4320 cycles @ 72 MHz = 1080 nops
#define NUMBER_NOPS_LONG 16  // 0.9 µs = 64.8 cycles @ 72 MHz = 16.2 nops --> 11.1 ns timing error
#define NUMBER_NOPS_SHORT 6  // 0.35 µs = 25.2 cycles @ 72 MHz = 6.3 nops --> 16.7 ns timing error


void ws2812_init(ws2812_init_typedef *control_struct, uint32_t port, uint16_t pin, uint32_t array_length, uint32_t matrix_width)
{
	control_struct->gpio_port = port;
	control_struct->gpio_pin = pin;
	control_struct->array_length = array_length;
	control_struct->matrix_width = matrix_width;
	control_struct->led_array = malloc(sizeof(uint8_t[array_length][3]));
	ws2812_clear_buffer(control_struct);
	ws2812_send_reset(control_struct);
}


void ws2812_send_reset(ws2812_init_typedef *control_struct)
{
	gpio_clear(control_struct->gpio_port, control_struct->gpio_pin);
	for (int i = 0; i < NUMBER_NOPS_RESET; i++) {
		__asm__("nop");
	}
}


void ws2812_send_zero(ws2812_init_typedef *control_struct)
{
	gpio_set(control_struct->gpio_port, control_struct->gpio_pin);
	for (int i = 0; i < NUMBER_NOPS_SHORT; i++) {
		__asm__("nop");
	}
	gpio_clear(control_struct->gpio_port, control_struct->gpio_pin);
	for (int i = 0; i < NUMBER_NOPS_LONG; i++) {
		__asm__("nop");
	}
}


void ws2812_send_one(ws2812_init_typedef *control_struct)
{
	gpio_set(control_struct->gpio_port, control_struct->gpio_pin);
	for (int i = 0; i < NUMBER_NOPS_LONG; i++) {
		__asm__("nop");
	}
	gpio_clear(control_struct->gpio_port, control_struct->gpio_pin);
	for (int i = 0; i < NUMBER_NOPS_SHORT; i++) {
		__asm__("nop");
	}
}


void ws2812_send_byte(ws2812_init_typedef *control_struct, uint8_t data)
{
	for (int i=7; i>=0; i--){
		if ( (data >> i) & 0x01 ){
			ws2812_send_one(control_struct);
		}
		else {
			ws2812_send_zero(control_struct);
		}
	}
}


void ws2812_send_led(ws2812_init_typedef *control_struct, uint8_t red, uint8_t green, uint8_t blue)
{
	ws2812_send_byte(control_struct, green);
	ws2812_send_byte(control_struct, red);
	ws2812_send_byte(control_struct, blue);
}


void ws2812_write_leds(ws2812_init_typedef *control_struct)
{
	for(uint32_t i=0; i<(control_struct->array_length); i++){
		ws2812_send_led(
			control_struct,
			control_struct->led_array[i][0],  // red
			control_struct->led_array[i][1],  // green
			control_struct->led_array[i][2]   // blue
		);
	}
	ws2812_send_reset(control_struct);
}


void ws2812_set_array_led(ws2812_init_typedef *control_struct, uint32_t index, uint8_t red, uint8_t green, uint8_t blue)
{
	control_struct->led_array[index][0] = red;
	control_struct->led_array[index][1] = green;
	control_struct->led_array[index][2] = blue;
}


void ws2812_set_matrix_led(ws2812_init_typedef *control_struct, uint32_t x, uint32_t y, uint8_t red, uint8_t green, uint8_t blue)
{
	ws2812_set_array_led(control_struct, (x + y * (control_struct->matrix_width)), red, green, blue);
}


void ws2812_clear_buffer(ws2812_init_typedef *control_struct)
{
	for (uint32_t i=0; i<control_struct->array_length; i++){
		ws2812_set_array_led(control_struct, i, 0, 0, 0);
	}
}
