

#include "fifo.h"


void fifo_init(FIFO *fifo, uint8_t *buffer, uint16_t bufferlength)
{
	fifo->buffer_ptr = buffer;
	fifo->bufferlength = bufferlength;
	fifo->read_index = 0;
	fifo->write_index = 0;
}

uint16_t fifo_get_buffersize(FIFO *fifo)
{
	return fifo->bufferlength;
}

uint16_t fifo_bytes_available(FIFO *fifo)
{
	if (fifo->write_index > fifo->read_index){
		return (fifo->write_index - fifo->read_index);
	}
	else if (fifo->write_index < fifo->read_index){
		return (fifo_get_buffersize(fifo) - (fifo->read_index - fifo->write_index));
	}
	else {  // read_index == write_index
		return 0;
	}
}

//uint16_t fifo_lines_available(FIFO *fifo){}

void fifo_move_read_index(FIFO *fifo)
{
	fifo->read_index++;
	fifo->read_index %= fifo_get_buffersize(fifo);
}

void fifo_move_write_index(FIFO *fifo)
{
	fifo->write_index++;
	fifo->write_index %= fifo_get_buffersize(fifo);
}

void fifo_get_byte(FIFO *fifo, uint8_t *data_sink)
{
	if(fifo_bytes_available(fifo)){
		*data_sink = fifo->buffer_ptr[fifo->read_index];
		fifo_move_read_index(fifo);
	}
}

void fifo_put_byte(FIFO *fifo, uint8_t *data_source)
{
	fifo->buffer_ptr[fifo->write_index] = *data_source;
	fifo_move_write_index(fifo);
	if(fifo->read_index == fifo->write_index){  // buffer overflow case
		fifo_move_read_index(fifo);
	}
}
