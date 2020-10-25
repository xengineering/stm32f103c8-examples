

/* 
    FIFO Ring Buffer uint8_t array ring_buffer (example length is 8) 
       with data (x) and read and write index:

     --- --- --- --- --- --- --- ---
    |   |   | x | x | x |   |   |   |
     --- --- --- --- --- --- --- ---
              ^           ^
              |           |
          read_index      |
                     write_index

     --- --- --- --- --- --- --- ---
    | x | x |   |   |   | x | x | x |
     --- --- --- --- --- --- --- ---
              ^           ^
              |           |
         write_index      |
                      read_index

    - input data goes to position of write_index
    - output data comes from read_index position
    - after input/output operation the according index has to be modified
    - according to the idea of ring buffers the write_index could be 
      smaller than the read_index
*/


#ifndef FIFO_H
#define FIFO_H


#include <stdint.h>
#include <stdlib.h>


typedef struct FIFO{
    uint8_t *buffer_ptr;
    uint16_t read_index;
    uint16_t write_index;
    uint16_t bufferlength;
} FIFO;


void fifo_init(FIFO *fifo, uint16_t bufferlength);
void fifo_deinit(FIFO *fifo);

uint16_t fifo_get_buffersize(FIFO *fifo);

uint16_t fifo_bytes_available(FIFO *fifo);
uint16_t fifo_lines_available(FIFO *fifo);

void fifo_move_read_index(FIFO *fifo);
void fifo_move_write_index(FIFO *fifo);

void fifo_get_byte(FIFO *fifo, uint8_t *data_sink);
void fifo_put_byte(FIFO *fifo, uint8_t *data_source);


#endif /* FIFO_H */
