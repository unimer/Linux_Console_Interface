/*
 * AVR244.h
 *
 *  Created on: Oct 24, 2017
 *      Author: Nikola Marin
 */

//#include <inavr.h>



/*** Cursor move direction ***/
#define MOVE_UP           'A'
#define MOVE_DOWN         'B'
#define MOVE_RIGHT        'C'
#define MOVE_LEFT         'D'



void Term_Send_Value_as_Digits( unsigned char value );
void Term_Move_Cursor( unsigned char distance, unsigned char direction );
//void LCD_Print_Data(int startX, int startY, int stopX, int stopY);
void LCD_Print_Data(int RX_cnt, unsigned char *RX_buffer);
void Term_Save_Cursor_Position();
void Term_Delete_Data();
void Term_Input_Data(unsigned char data);
void Term_Output_Display(unsigned char data);
