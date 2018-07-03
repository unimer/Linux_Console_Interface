/*
 * term_display.h
 *
 *  Created on: Oct 31, 2017
 *      Author: root
 */

#define LCD_MAX_ROW 22
#define LCD_MAX_COLUMN 20


typedef struct
{
	unsigned char tail;
	unsigned char head;
	unsigned char row;
	unsigned char col;
	unsigned char counter;
	unsigned char display[LCD_MAX_COLUMN][LCD_MAX_ROW];
}lcd_matrix;

//unsigned char display[LCD_MAX_COLUMN][LCD_MAX_ROW];

lcd_matrix lm;


void term_init(lcd_matrix *matrix);
unsigned char lf(unsigned char letter);
unsigned char cr(unsigned char letter);

void term_put_data(lcd_matrix *matrix, unsigned char data);
unsigned char term_get_data(unsigned char *disp, int i);
void lcd_set_cursor(unsigned char characters, unsigned char position);
void terminal_page_up(lcd_matrix *matrix);
void terminal_page_down(lcd_matrix *matrix);
void lcd_delete_data(lcd_matrix *matrix);
void lcd_print_data(lcd_matrix *matrix,unsigned char begin, unsigned char end);
