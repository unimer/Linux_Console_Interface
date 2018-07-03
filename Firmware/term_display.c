/*
 * term_display.c
 *
 *  Created on: Oct 31, 2017
 *      Author: root
 */

#include "term_display.h"
#include "uart.h"
#include "lcd.h"
#include "keypad.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "AVR244.h"
#include <util/delay.h>



void term_init(lcd_matrix *matrix)
{
	matrix->tail = 0;
	matrix->head = 0;
	matrix->col = 0;
	matrix->row = 0;
}



unsigned char lf(unsigned char letter)
{
	if ((letter == 10))
	{
		return 1;
	}
	else return 0;
}



unsigned char cr(unsigned char letter)
{
	if ((letter == 13))
	{
		return 1;
	}
	else return 0;
}

int full_circle(int i)
{
	if (i >= LCD_MAX_COLUMN - 1) return (LCD_MAX_COLUMN - 1);
	else return 0;
}



void term_put_data(lcd_matrix *matrix,unsigned char data)
{


	int i = 0;

	if (matrix->row >= LCD_MAX_ROW - 1)
		{
			matrix->row = 0;
			matrix->col ++;

		}

	if (matrix->col >= LCD_MAX_COLUMN - 1)
	{
		matrix->row = 0;
		for (int i = 0; i <= LCD_MAX_COLUMN - 1; i++)
		{
			strcpy((char *)matrix->display[i],(const char *) matrix->display[i + 1]);
		}

		matrix->col -- ;
	}


	if(lf(data))
	{

		matrix->display[matrix->col][matrix->row] = data;
		matrix->display[matrix->col][matrix->row + 1 ] = '\0';
		matrix->row = 0;
		matrix->col ++;

	}
	else if (keypad.pos_cnt >= 1)
	{
		for (i = matrix->row; i>= (matrix->row - keypad.pos_cnt); --i)
		{
			matrix->display[matrix->col][i + 1] = matrix->display[matrix->col][i] ;
		}

		matrix->display[matrix->col][matrix->row - keypad.pos_cnt] = data;
		matrix->row++;

	}
	else
	{
		matrix->display[matrix->col][matrix->row] = data;
		matrix->display[matrix->col][matrix->row + 1 ] = '\0';
		matrix->row ++;

	}



	matrix->head = matrix->col;
	if (matrix->head > 7)
	{
		matrix->tail = matrix->head - 7;
	}
	GLCD_cursor_premission(enable);


}



void terminal_page_up(lcd_matrix *matrix)
{
	if ( matrix->head > 7)
	{

			matrix->head--;

			matrix->tail = matrix->head - 7;

			GLCD_remove_cursor(&xy);
			GLCD_cursor_premission(disable);
			lcd_print_data(matrix, matrix->tail, matrix->head);



	}

;

}

void terminal_page_down(lcd_matrix *matrix)
{
	if ( matrix->head < matrix->col)
	{
		matrix->head ++;

		matrix->tail = matrix->head - 7;

		GLCD_remove_cursor(&xy);
		GLCD_cursor_premission(disable);
		lcd_print_data(matrix, matrix->tail, matrix->head);

	}
}

void lcd_delete_data(lcd_matrix *matrix)
{
	//matrix->row --;
	int i = 0;


	if (strlen((const char*)matrix->display[matrix->col]) == 0)
	{
		matrix->col--;
		matrix->row = strlen((const char*)matrix->display[matrix->col]);

	}
	else
	{
		matrix->row=strlen((const char*)matrix->display[matrix->col]);
	}


		for (i = ((matrix->row-1) - keypad.pos_cnt); i < matrix->row; i++)
		{
			matrix->display[matrix->col][i] = matrix->display[matrix->col][i + 1];

		}

		matrix->row = strlen((const char*)matrix->display[matrix->col]);


}


void lcd_print_data(lcd_matrix *matrix, unsigned char begin, unsigned char end)
{
		unsigned char data;
		int len;

		GLCD_clr();
		xy.x = 1;
		xy.y = 0;
		GOTO_xy(xy.x, xy.y);

		for (int i = begin; i <= end; i++)
		{
			len = strlen((const char*)matrix->display[i]);

			if (len > 0)
			{

				for (int j=0; j < len; j++)
				{
					data = matrix->display[i][j];

					if (!(cr(data)))
					{

						if (lf(data))
						{
							xy.x = 1;
							xy.y++;
							GOTO_xy(xy.x,xy.y);
							break;
						}
						else
						{
							put_ch(data, xy.x, xy.y);
							GLCD_move_cursor(&xy);
						}

					}

				}

			}

		}

		if (keypad.pos_cnt >= 1)
		{
			xy.x = xy.x - ((keypad.pos_cnt * 6) + 1);
		}
		else xy.x = xy.x - ((keypad.pos_cnt * 6));
		GOTO_xy(xy.x, xy.y);

		if (xy.y > 7)
		{
			lm.tail++;
			lcd_print_data(&lm,lm.tail,lm.head);
		}






}












