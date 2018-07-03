#include <avr/interrupt.h>
#include <string.h>
#include "uart.h"
#include <avr/io.h>
#include "lcd.h"
#include "term_display.h"
#include "keypad.h"
#include <stdlib.h>


#define RX_INTERRUPT USART_RXC_vect
#define TX_INTERRUPT USART_UDRE_vect
#define TX_INTERRUPT_ENABLE() 	UCSRB |= (1<<UDRIE)|(1<<TXCIE);	 UCSRA |= (1<<UDRE);
#define TX_INTERRUPT_DISABLE()  UCSRB &= ~(1<<UDRIE); UCSRB &= ~(1<<TXCIE); UCSRA &= (1<<UDRE);



/* Ring Buffer Functions and declarations*/
void RB_init(struct ring_buffer_t *rb )
{
	rb->head = 0;
	rb->tail = 0;
}

int ringBufNewData(struct ring_buffer_t *apBuffer)
{
	if (apBuffer->head != apBuffer->tail)
	{
		return 1;
	}
	else return 0;
}


char ringBufGetChar (struct ring_buffer_t *apBuffer)
{
    int index;
    index = apBuffer->head;
    apBuffer->head = (apBuffer->head + 1) % RING_BUFFER_SIZE;
    return apBuffer->data[index];
}

void ringBufPutChar (struct ring_buffer_t *apBuffer, const char c)
{
    apBuffer->data[apBuffer->tail] = c;
    apBuffer->tail = (apBuffer->tail + 1) % RING_BUFFER_SIZE;
}



void put_screen(void)
{
	unsigned char data;
	//if (RB_length(&rb_rx) > 0 )
	//{
	if (ringBufNewData(&rb_rx))
	{

		data = ringBufGetChar(&rb_rx);
		if (data == 27)
		{
			//data = ringBufGetChar(&rb_rx);

				ringBufGetChar(&rb_rx);
				ringBufGetChar(&rb_rx);
				ringBufGetChar(&rb_rx);
				if (ringBufNewData(&rb_rx))
				{
					data = ringBufGetChar(&rb_rx);
					//if (keypad.pos_cnt == 1) ringBufGetChar(&rb_rx);
				}

		}


		if ((data != 8) && (data != 27) && (data != 7))
		{
			term_put_data(&lm,data);
			lcd_print_data(&lm,lm.tail, lm.head );
		}
		else if(data == 8)
		{
			lcd_delete_data(&lm);
			lcd_print_data(&lm,lm.tail, lm.head );

		}



	}


}



/*UART communication functions and declarations*/


void uart_init(uint16_t ubrr)
{


	/*Set Baud Rate*/
	UBRRH = (uint8_t) (ubrr>>8);
	UBRRL = (uint8_t) ubrr;

	RB_init(&rb_rx);
	RB_init(&rb_tx);

		/* Set Framing error(FE), Data OverRun(DOR) */
		UCSRA |= (1<<FE)|(1<<DOR)|(1<<UDRE);

		/*Enable RX Complete Interrupt, receiver and transmitter*/
		UCSRB |= (1<<RXCIE)|(1<<TXCIE)|(1<<TXEN)|(1<<RXEN);

		/*Set frame formatL 9 Data bits, 1 Stop bit*/
		UCSRC |= (1<<URSEL)|(1<<USBS)|(3<<UCSZ0);

}

void uart_Transmit(unsigned char data)
{


	//RB_writeByte(&rb_tx, data);
	ringBufPutChar(&rb_tx, (const char)data);

	TX_INTERRUPT_ENABLE();


}

ISR(TX_INTERRUPT)
{

		UDR = ringBufGetChar(&rb_tx);
		TX_INTERRUPT_DISABLE();

}

ISR (RX_INTERRUPT)
{
	unsigned char status;

	status  = UCSRA;
	if ( ((status & (FE | DOR) ) == 0 ) )
	{
		ringBufPutChar(&rb_rx,(const char)UDR);


	}


}
