#ifndef LiquidCrystal_h
#define LiquidCrystal_h

#include <stdint.h>

// commands
#define LCD_CLEARDISPLAY 				0x01
#define LCD_RETURNHOME 					0x02
#define LCD_ENTRYMODESET 				0x04
#define LCD_DISPLAYCONTROL 			0x08
#define LCD_CURSORSHIFT 				0x10
#define LCD_FUNCTIONSET 				0x20
#define LCD_SETCGRAMADDR 				0x40
#define LCD_SETDDRAMADDR 				0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT 					0x00
#define LCD_ENTRYLEFT 					0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON 					0x04
#define LCD_DISPLAYOFF 					0x00
#define LCD_CURSORON 						0x02
#define LCD_CURSOROFF 					0x00
#define LCD_BLINKON 						0x01
#define LCD_BLINKOFF 						0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 				0x08
#define LCD_CURSORMOVE 					0x00
#define LCD_MOVERIGHT 					0x04
#define LCD_MOVELEFT 						0x00

// flags for function set
#define LCD_8BITMODE 						0x10
#define LCD_4BITMODE 						0x00
#define LCD_2LINE 							0x08
#define LCD_1LINE 							0x00
#define LCD_5x10DOTS 						0x04
#define LCD_5x8DOTS 						0x00


//functions

void LCD_init(uint8_t fourbitmode, uint8_t rs, uint8_t rw, uint8_t enable,
		uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3,
		uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7);
	

void LCD_clear();
void LCD_home();

void LCD_noDisplay();
void LCD_display();
void LCD_Blink();
void LCD_blink();
void LCD_noCursor();
void LCD_cursor();
void LCD_scrollDisplayLeft();
void LCD_scrollDisplayRight();
void LCD_leftToRight();
void LCD_rightToLeft();
void LCD_autoscroll();
void LCD_noAutoscroll();

void LCD_setRowOffsets(int row1, int row2, int row3, int row4);
void LCD_createChar(uint8_t, uint8_t[]);
void LCD_setCursor(uint8_t, uint8_t); 
void LCD_command(uint8_t);

void LCD_send(uint8_t, uint8_t);
void LCD_write4bits(uint8_t);
void LCD_write8bits(uint8_t);
void LCD_pulseEnable();

#endif
