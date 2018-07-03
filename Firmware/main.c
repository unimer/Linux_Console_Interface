#define F_CPU 11059200UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include "lcd.h"
#include "keypad.h"
#include "uart.h"
#include "AVR244.h"
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <stdio.h>
#include "term_display.h"




int main(void)
{


	DDRA = 0xFF; //LCD data
	DDRB |= 0xFF;
	DDRD |= 0x7F; //cs1

	_delay_us(1);
	set_cs1(1);
	set_cs2(1);
	set_rst(1);
	_delay_us(1);
	GLCD_ON();
	Set_Start_Line(0);
	_delay_us(50);
	GLCD_clr();


	put_init_message(15, 5); //init message


	KeyboardInit();
	uart_init(UART_BAUD_SELECT(9600, F_CPU));
	term_init(&lm);


	sei(); //Enable global interrupt


	while(1){

		GLCD_cursor(&xy);
		put_screen();

		KeyboardTasks();

	}





}
