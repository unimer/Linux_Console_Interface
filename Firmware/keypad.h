/*
 * keypad.h
 *
 * Created: 5/29/2017 9:27:45 PM
 *  Author: Dzoni
 */

#include<avr/io.h>

//define row output and input port
#define ROW_WRITE PORTD
#define ROW_READ PIND
#define ROW_DDR DDRD

//define column output and input port
#define COL_WRITE PORTC
#define COL_READ PINC
#define COL_DDR	DDRC

//define keypad LEDs port
#define LED_DDR DDRB
#define LED_WRITE PORTB
#define LED_READ PINB


typedef enum
{
	false, true
}bool;

typedef enum
{
	LETTERS,
	NUMBERS_AND_SYMBOLS,
	SYMBOLS
}keypad_state;

typedef struct
{
	unsigned char char_cnt;
	unsigned char pos_cnt;
}kp_data;

kp_data keypad;

//char read_map(bool shift, keypad_state stat, int ROW, char column);
char read_key(bool shift, keypad_state stat);

void KeyboardInit(void);
char GetCharacter(char row, char column);

void EnterPressed(void);
void MoveCursorPressed(void);
void BackspacePressed(void);
//void TextManage(char character, char X, char Y);
void KeyboardTasks(void);
