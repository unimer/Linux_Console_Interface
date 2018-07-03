#include<avr/io.h>

//LCD control bits setup
#define GLCD_CS1 PD2
#define GLCD_CS2 PB4
#define GLCD_RS  PB3
#define GLCD_RW  PB2
#define GLCD_EN  PB1
#define GLCD_RST PB0

//LCD data port setup
#define GLCD_DATA_PORT PORTA
#define GLCD_DATA_PORT_READING PINA

//preprocessing GLCD_DATA_PORT
#ifndef GLCD_DATA_PORT
# warning "GLCD_DATA_PORT is not defined for glcd.h.Default Port Register is PORTA"
#define GLCD_DATA_PORT PORTA
#endif

//preprocessing GLCD_DATA_READING
#ifndef GLCD_DATA_PORT_READING
# warning "GLCD_DATA_PORT_READING is not defined for glcd.h.Default Port Register is PORTA"
#define GLCD_DATA_PORT_READING PINA

//preprocessing GLCD_CS1
# warning "GLCD_CS1 is not defined for glcd.h.Default Port Register is PD1"
#define GLCD_CS1 PD2
#endif

//preprocessing GLCD_CS2
#ifndef GLCD_CS2
# warning "GLCD_CS2 is not defined for glcd.h.Default Port Register is PB4"
#define GLCD_CS2 PB4
#endif

//preprocessing GLCD_RS
#ifndef GLCD_CS2
# warning "GLCD_RS is not defined for glcd.h.Default Port Register is PB3"
#define GLCD_CS2 PB3
#endif

//preprocessing GLCD_RW
#ifndef GLCD_RW
# warning "GLCD_RW is not defined for glcd.h.Default Port Register is PB2"
#define GLCD_RW PB2
#endif

//preprocessing GLCD_EN
#ifndef GLCD_EN
# warning "GLCD_EN is not defined for glcd.h.Default Port Register is PB1"
#define GLCD_EN PB1
#endif

//preprocessing GLCD_RST
#ifndef GLCD_CS1
# warning "GLCD_RST is not defined for glcd.h.Default Port Register is PB0"
#define GLCD_RST PB0
#endif

#ifndef LCD_H_
#define LCD_H_

#endif /* LCD_H_ */

struct GLCD_coordinates
{
	unsigned char x;
	unsigned char y;
};

typedef enum
{
	enable, disable
}premission;


struct GLCD_coordinates xy;



void set_rs(unsigned short val);
void set_rw(unsigned short val);
void set_en(unsigned short val);
void set_cs1(unsigned short val);
void set_cs2(unsigned short val);
void set_rst(unsigned short val);
void enable_pulse(void);

//setting LCD instructions


void GLCD_write(unsigned char b);
void GLCD_ON(void);
void Set_Start_Line(char line);
void GOTO_col(unsigned char x);
void GOTO_page(unsigned short y);
void GOTO_xy(unsigned int x, unsigned int y);
void GLCD_cursor(struct GLCD_coordinates *xy);

void GLCD_clr_p2p(char X1, char Y1, char X2, char Y2);
void GLCD_clr_line(unsigned int ln);
void GLCD_clr(void);

void GLCD_cursor_premission(premission prem);
void GLCD_remove_cursor(struct GLCD_coordinates *xy);
void GLCD_move_cursor(struct GLCD_coordinates *xy);
unsigned char is_busy();
unsigned char GLCD_read(int col);
void point_at(int x, int y, int color);
void h_line(unsigned int x, unsigned int y, unsigned short l, unsigned short c);
void v_line(unsigned int x, unsigned int y, unsigned short l, unsigned short c);

//structures
//extern const struct FONT minionPro_8ptDesc[];
//extern const unsigned char minionPro_8ptBitmaps[];
extern const unsigned char Terminal5x8[];

//text writing functions
//void put_ch(char c, unsigned char x, unsigned char y);
void put_ch(char c, unsigned char x, unsigned char y);
void put_init_message(unsigned char x, unsigned char y);
