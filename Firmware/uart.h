/*
 * uart.h
 *
 * Created: 10/16/2017 7:01:09 PM
 *  Author: Dzoni
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdlib.h>
#define UART_BAUD_SELECT(baudRate,xtalCpu) (((xtalCpu)+8UL*(baudRate))/(16UL*(baudRate))-1UL)
#define RING_BUFFER_SIZE 256


struct ring_buffer_t
{
	unsigned int head;
	unsigned int tail;
	unsigned char data[RING_BUFFER_SIZE];

};

struct ring_buffer_t rb_tx;
struct ring_buffer_t rb_rx;

void RB_init(struct ring_buffer_t *rb);
int ringBufNewData(struct ring_buffer_t *apBuffer);
char ringBufGetChar (struct ring_buffer_t *apBuffer);
void ringBufPutChar (struct ring_buffer_t *apBuffer, const char c);

void put_screen(void);
void uart_init(uint16_t baudrate);
void uart_Transmit(unsigned char data);
