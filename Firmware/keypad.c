/*
 * keypad.c
 *
 * Created: 5/29/2017 9:34:47 PM
 *  Author: Dzoni
 */
#include "keypad.h"
#include "lcd.h"
#include "uart.h"
#include "avr/pgmspace.h"
#define F_CPU 11059200UL
#include <util/delay.h>
#include <string.h>
#include "AVR244.h"
#include "term_display.h"



static keypad_state stat;
bool shift_state = false;

const char keymap[] PROGMEM = {
	't',	//index = 0; row = 0; col = 0;	//=========IDLE===========
	'b',	//index = 1; row = 3; col = 0;//end for
	'n',	//index = 2; row = 2; col = 0;
	'g',	//index = 3; row = 1; col = 0;
	'y',	//index = 4; row = 0; col = 1;
	' ',	//index = 5; row = 3; col = 1; SPACE
	'o',	//index = 6; row = 4; col = 1;
	'h',	//index = 7; row = 1; col = 1;
	'u',	//index = 8; row = 0; col = 2;
	 7,		//index = 9; row = 3; col = 2; KEY SELECT
	'p',	//index = 10; row = 2; col = 2;
	'i',	//index = 11; row = 1; col = 2;
	 8,		//index = 12; row = 0; col = 3;	BACKSPACE
	17,		//index = 13; row = 3; col = 3;	Device Control
	15,		//index = 14; row = 2; col = 3; Shift
	13,		//index = 15; row = 1; col = 3; ENTER(Carriage return)
	'v',	//index = 16; row = 0; col = 4;
	'r',	//index = 17; row = 1; col = 4;
	'f',	//index = 18; row = 2; col = 4;
	'm',	//index = 19; row = 3; col = 4;
	'c',	//index = 20; row = 0; col = 5;
	'e',	//index = 21; row = 1; col = 5;
	'd',	//index = 22; row = 2; col = 5;
	'l',	//index = 23; row = 3; col = 5;
	'x',	//index = 24; row = 0; col = 6;
	'w',	//index = 25; row = 1; col = 6;
	's',	//index = 26; row = 2; col = 6;
	'k',	//index = 27; row = 3; col = 6;
	'z',	//index = 28; row = 0; col = 7;
	'q',	//index = 29; row = 1; col = 7;
	'a',	//index = 30; row = 2; col = 7;
	'j',	//index = 31; row = 3; col = 7;
	'7',	//index = 32; row = 0; col = 0;	//=========Numbers and symbols===========
	'0',	//index = 33; row = 3; col = 0;
	'1',	//index = 34; row = 2; col = 0;
	'4',	//index = 35; row = 1; col = 0;
	'8',	//index = 36; row = 0; col = 1;
	' ',	//index = 37; row = 3; col = 1;
	'2',	//index = 38; row = 2; col = 1;
	'5',	//index = 39; row = 1; col = 1;
	'9',	//index = 40; row = 0; col = 2;
	 7,	    //index = 41; row = 3; col = 2;	//Keypad Select
	'3',	//index = 42; row = 2; col = 2;
	'6',	//index = 43; row = 1; col = 2;
	 8,		//index = 44; row = 0; col = 3;	//BACKSPACE
	17,		//index = 45; row = 3; col = 3; //Device CTRL
	15,		//index = 46; row = 2; col = 3;	//SHIFT
	13,		//index = 47; row = 1; col = 3; //ENTER (Carriage return);
	'!',	//index = 48; row = 0; col = 4;
	'=',	//index = 49; row = 1; col = 4;
	'&',	//index = 50; row = 2; col = 4;
	'\"',	//index = 51; row = 3; col = 4;  //""
	'?',	//index = 52; row = 0; col = 5;
	':',	//index = 53; row = 1; col = 5;
	'$',	//index = 54; row = 2; col = 5;
	'\'',	//index = 55; row = 3; col = 5; //''
	',',	//index = 56; row = 0; col = 6;
	'/',	//index = 57; row = 1; col = 6;
	 19,	//index = 58; row = 2; col = 6; //Page DN
	'@',	//index = 59; row = 3; col = 6;
	'.',	//index = 60; row = 0; col = 7;
	'-',	//index = 61; row = 1; col = 7;
	 18,	//index = 62; row = 2; col = 7; //Page UP
	'_',	//index = 63; row = 3; col = 7;
	'(',	//index = 64; row = 0; col = 0; //=========SYMBOLS===========
	'@',	//index = 65; row = 3; col = 0;
	'\\',	//index = 66; row = 2; col = 0;	//backslash
	'<',	//index = 67; row = 1; col = 0;
	')',	//index = 68; row = 0; col = 1;
	' ',	//index = 69; row = 3; col = 1; //space
	'^',	//index = 70; row = 2; col = 1;
	'>',	//index = 71; row = 1; col = 1;
	'=',	//index = 72; row = 0; col = 2;
	 7,		//index = 73; row = 3; col = 2; //Keypad select
	'&',	//index = 74; row = 2; col = 2;
	'~',	//index = 75; row = 1; col = 2;
	 8,		//index = 76; row = 0; col = 3; //Backspace
	17,		//index = 77; row = 3; col = 3;	//Device Control
	15,		//index = 78; row = 2; col = 3;	//Shift
	13,	    //index = 79; row = 1; col = 3; //Enter (Carriage return)
	'!',	//index = 80; row = 0; col = 4;
	'}',	//index = 81; row = 1; col = 4;
	'/',	//index = 82; row = 2; col = 4;
	'|',	//index = 83; row = 3; col = 4;
	'?',	//index = 84; row = 0; col = 5;
	'{',	//index = 85; row = 1; col = 5;
	'*',	//index = 86; row = 2; col = 5;
	'$',	//index = 87; row = 3; col = 5;
	',',	//index = 88; row = 0; col = 6;
	']',	//index = 89; row = 1; col = 6;
	'-',	//index = 90; row = 2; col = 6;
	'#',	//index = 91; row = 3; col = 6;
	'.',	//index = 92; row = 0; col = 7;
	'[',	//index = 93; row = 1; col = 7;
	'+',	//index = 94; row = 2; col = 7;
	'%',	//index = 95; row = 3; col = 7;
};
/*


Detect active bit and convert it into number. */
static int BitPositionToNumber(unsigned char bit)
{
	switch(bit)
	{
		case 0x01:
			return 0;
		case 0x02:
			return 1;
		case 0x04:
			return 2;
		case 0x08:
			return 3;
		case 0x10:
			return 4;
		case 0x20:
			return 5;
		case 0x40:
			return 6;
		case 0x80:
			return 7;
		default:
			return 8;
	}
}

/* Initialize keyboard. */
void KeyboardInit(){
	//set row pins as output
	ROW_DDR = 0x7F;

	//set column pins as input
	COL_DDR = 0x00;

	//set LED port as output;
	LED_DDR = 0xFF;
	LED_WRITE = (LED_READ | 0xE0);

	xy.x = 1;
	xy.y = 0;
	stat = LETTERS;

	keypad.char_cnt= 0;
	keypad.pos_cnt = 0;

}

char GetCharacter(char row, char column)
{
	char character;
	char read;

		switch (stat)
		{
			case LETTERS:

				character =	pgm_read_byte(&keymap[(row)+( BitPositionToNumber(column) * 4 )]);

				if (shift_state == true)
				{
					if (character == 17 || character == 13 || character == 8 )
					{
						read = character;
					}
					else	read = character - 32;
				}
				else read = character;


				if (character == 15)		//if shift is pressed
				{
					if (shift_state == false) shift_state = true;
					else if (shift_state == true) shift_state = false;
				}
				else if (character == 7)	//if NUM is pressed
				{
					stat = NUMBERS_AND_SYMBOLS;
					shift_state = false;
					LED_WRITE = (LED_READ & 0xBF); //NUM/SYMB LED ON

				}
				else return read;

			break;
			case NUMBERS_AND_SYMBOLS:
			    character = pgm_read_byte(&keymap[((row)+( BitPositionToNumber(column) * 4 )+32)]);

				if (character == 15)
				{
					if (shift_state == false) shift_state = true;
					else if (shift_state == true) shift_state = false;
					stat = SYMBOLS;
					LED_WRITE = (LED_READ & 0xBF); //NUM/SYMB LED ON
				}
				else if (character == 7)
				{
					shift_state = false;
					stat = LETTERS;
					LED_WRITE = (LED_READ | 0X40);//NUM led off
				}
				else return character;

			break;
			case SYMBOLS:
				character =	pgm_read_byte(&keymap[((row)+( BitPositionToNumber(column) * 4 )+64)]);

				if (character == 15)
				{
					if (shift_state == false) shift_state = true;
					else if (shift_state == true) shift_state = false;
					stat = NUMBERS_AND_SYMBOLS;
					LED_WRITE = (LED_READ & 0xBF); //NUM/SYMB LED ON
				}
				else if (character == 7)
				{
					shift_state = false;
					stat = LETTERS;
					LED_WRITE = (LED_READ | 0X40);//NUM led off
				}
				else return character;
			break;
		}

		switch (shift_state)
		{
			case false:
				LED_WRITE = ( LED_READ | 0x80); //shift LED OFF
			break;
			case true:
				LED_WRITE = (LED_READ & 0x7F); //shift LED on
			break;
		}

	return 0;
}

void EnterPressed(void)
{

	//keypad.data[keypad.char_cnt] = 13;
	uart_Transmit(13);
	keypad.pos_cnt = 0;

}


void MoveCursorPressed()
{

	//lcd_set_cursor(keypad.char_cnt, keypad.pos_cnt, shift_state);
	GLCD_remove_cursor(&xy);

		switch(shift_state)
			{
				case false:

						if (keypad.char_cnt != keypad.pos_cnt)
						{


							if (xy.x == 0 || (xy.x == 1 && keypad.char_cnt != 0))
							{
								xy.x = 120;
								xy.y --;
							}
							else if (keypad.pos_cnt >= 1)
							{
								xy.x -= 6;
							}
							else xy.x -= 7;

							keypad.pos_cnt += 1;
							Term_Move_Cursor(1,MOVE_LEFT);

						}

				break;
				case true:

					if (keypad.pos_cnt != 0)
					{

						keypad.pos_cnt -= 1;
						if (xy.x > 119)
						{
							if (keypad.pos_cnt == 0)
							{
								xy.x = 1;
								xy.y ++;
							}
							else
							{
								xy.x = 0;
								xy.y ++;
							}

						}
						else if (keypad.pos_cnt >= 1)
						{
							xy.x += 6;
						}
						else xy.x += 7;
						Term_Move_Cursor(1,MOVE_RIGHT);
					}


				break;
			}

			GOTO_xy(xy.x, xy.y);

}

void BackspacePressed()
{
	uart_Transmit(8);


}



/* Check if some button is pressed... */
void KeyboardTasks(void){
	unsigned char column;

	static char last_column;


	char current_row = 0;
	char current_column = 0;
	unsigned char character;



	/* Detect is any button pressed by checking every row and column combination. */
	for(int row=0; row<4; row++){

		ROW_WRITE = ((ROW_READ & 0x87) | 0x78) & (~(1<<(row+3)));
		column = ~COL_READ;

		/* Button in current row and column is pressed, if two buttons pressed than ignore. */
		if(column && BitPositionToNumber(column)<8)
		{
			current_row= row;
			current_column=column;
		}
	}

	/* Ignore button press if button was held down. */
	if(current_column && last_column != current_column){


		character = GetCharacter(current_row, current_column);
		if (character != 0)
		{
			if (character == 13)
			{
				EnterPressed();
			}
			else if(character == 17)
			{
				MoveCursorPressed();
			}
			else if(character == 8)
			{
				BackspacePressed();
			}
			else if(character == 18)
			{
				terminal_page_up(&lm);
			}
			else if(character == 19)
			{
				terminal_page_down(&lm);
			}
			else
			{
				keypad.char_cnt++;
				uart_Transmit(character);

				if (lm.head > 7)
				{
						lm.tail = lm.head - 7;
						lcd_print_data(&lm, lm.tail, lm.head);

				}


			}

		}




	}

	//last_row = current_row;
	last_column = current_column;
	_delay_us(3000);
}
