#ifndef __LDC5110_Basic_H__
#define __LDC5110_Basic_H__

#include "TM4C129.h"                    // Device header

#define LCD_COMMAND	0
#define LCD_DATA		1

//General Commands
#define PCD8544_FUNCTIONSET						0x20
#define PCD8544_POWERDOWN							0x04
#define PCD8544_ENTRYMODE							0x02
#define PCD8544_EXTENDEDINSTRUCTION		0x01

//Display Control-Normal Instructions
#define PCD8544_DISPLAYCONTROL				0x08
#define PCD8544_DISPLAY_D							0x04
#define PCD8544_DISPLAY_E							0x01
#define PCD8544_DISPLAYBLANK					0x00
#define PCD8544_DISPLAYNORMAL					PCD8544_DISPLAY_D
#define PCD8544_DISPLAYALLDISPSEGON		PCD8544_DISPLAY_E
#define PCD8544_DISPLAYINVVIDEO				(PCD8544_DISPLAY_D | PCD8544_DISPLAY_E)

#define PCD8544_SETXADDR							0x80 //LSB 7 bits
#define PCD8544_SETYADDR							0x40 //LSC 3 bits

//Extended Instructions
#define PCD8544_SETTEMPCOEF						0x04	//LSB 2 bits
#define PCD8544_SETBIASSYS						0x10	//LSB 3 bits
#define PCD8544_SETVOP								0x80 	//LSB 7 bits

//Initial Value
#define LCD_BIAS											0x03	//Range 0-7 1:48
#define LCD_TEMP											0x02	//Range 0-3
#define LCD_CONTRAST									00x46	//Range 0-127


#define LEFT 0
#define RIGHT 9999
#define CENTER 9998


#define bitmapdatatype unsigned int*


typedef enum {False=0, True=1} boolean;	

typedef struct __current_font{

	unsigned char *font;
	unsigned char x_size;
	unsigned char y_size;
	unsigned char offset;
	unsigned char numchars;
	unsigned char inverted;
	
} __current_font;




void GPIO_init(void);
void SSI_init(void);
void SSI2Write(unsigned char data);
void MCU_start(void);
void resetLCD(void);
void __LCD_WRITE(unsigned char data, unsigned char mode);
void LCD5510_initLCD(int contrast);
unsigned char fontbyte(unsigned int x);
void __print_char(unsigned char c, int x, int row);
void LCD5110_print(char *st, int x, int y);
void LCD5110_printNumI(long num, int x, int y, int length, char filler);
void __convert_float(char *buf, double num, int width, unsigned char prec);
void LCD5110_printNumF(double num, unsigned char dec, int x, int y, char divider, int length, char filler);
unsigned char bitmapbyte(unsigned int x, bitmapdatatype bitmap);
void LCD5110_drawBitmap(int x, int y, bitmapdatatype bitmap, int sx, int sy);
void LCD5110_clrScr(void);
void LCD5110_clrRow(int row, int start_x, int end_x);
void LCD5110_invert(unsigned char mode);
void LCD5110_invertText(unsigned char mode);
void LCD5110_setFont(unsigned char *font);
void LCD5110_enableSleep(void);
void LCD5110_disableSleep(void);












#endif //__LDC5110_Basic_H__

