/*
 * AVR244.c
 *
 *  Created on: Oct 24, 2017
 *      Author: Nikola Marn
 */

#include "AVR244.h"
#include "uart.h"
#include "lcd.h"
#include <util/delay.h>

#define SENDESC	\
    uart_Transmit( 27 );	\
    uart_Transmit( '[' );

struct Cursor_Start_Coordinates
{
	unsigned char x;
	unsigned char y;
};

struct  Cursor_Start_Coordinates cursorxy;

//unsigned char LCD_DATA[50][20];

//unsigned char RX_buffer[128];

void Term_Send_Value_as_Digits(unsigned char value )
{
    char digit;

    digit = '0';
    while( value >= 100 )                // Still larger than 100 ?
    {
        digit++;                         // Increment first digit
        value -= 100;
    }

    uart_Transmit(digit);                  // Send first digit

    digit = '0';
    while( value >= 10 )                 // Still larger than 10 ?
    {
        digit++;                         // Increment second digit
        value -= 10;
    }

    uart_Transmit( digit );                  // Send second digit

    uart_Transmit( '0' + value );            // Send third digit
}

/*void Term_Set_Cursor_Position( unsigned char row, unsigned char column )
{
    SENDESC                                        // Send escape sequence start

    Term_Send_Value_as_Digits( row );              // Convert row byte
    uart_Transmit( ';' );
    Term_Send_Value_as_Digits( column );           // Convert column byte
    uart_Transmit( 'H' );
}*/


void Term_Move_Cursor( unsigned char distance, unsigned char direction )
{
	UCSRB &=~(1<<RXCIE);
	UCSRB &=~(1<<RXEN);
    SENDESC                            // Send escape sequence start

    //Term_Send_Value_as_Digits( distance );         // Convert distance byte

    uart_Transmit( direction );
    _delay_ms(50);
	UCSRB |=(1<<RXCIE);
	UCSRB |=(1<<RXEN);
}

void Term_Save_Cursor_Position()
{
    SENDESC                             // Send escape sequence start

    uart_Transmit( '?');
    uart_Transmit( '8');
    uart_Transmit( 'h');
}


void Term_Delete_Data()
{
	SENDESC
	Term_Send_Value_as_Digits( 1 );
	uart_Transmit('P');
}
/*void LCD_Print_Data(int startX, int startY, int stopX, int stopY)
{
	int i = 0;
	int j = 0;
	GLCD_clr();
	xy.x = 1;
	xy.y = 0;
	GOTO_xy(xy.x, xy.y);

	for (i = startY; i < stopY; i++)
	{
		if(LCD_DATA[i][j] == 10)
		{
			continue;
		}
		for (j = startX; j < stopX; j++)
		{

			put_ch(LCD_DATA[i][j], xy.x, xy.y);
			GLCD_move_cursor(&xy);
		}
	}
}*/

void LCD_Print_Data(int RX_cnt, unsigned char *RX_buffer)
{
	GLCD_clr();
	xy.x = 1;
	xy.y = 0;
	GOTO_xy(xy.x, xy.y);

	/*for (int i = 0; i < RX_cnt; i++)
	{
		put_ch(RX_buffer[i], xy.x, xy.y);
		GLCD_move_cursor(&xy);
	}*/
}


void Term_Input_Data(unsigned char data)
{



	//put_ch(data, xy.x, xy.y);
	//GLCD_move_cursor(&xy);


}


