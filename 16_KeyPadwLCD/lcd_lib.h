#ifndef __LCD_LIB_H__
#define __LCD_LIB_H__


#define Reg_Select 	(1U<<0) //0b 0000 0001
#define Read_Write 	(1U<<1) //0b 0000 0010
#define Enable			(1U<<2) //0b 0000 0100


//wait 1520 us 
#define clc_disp			0x01	//Clear Display Screen
#define return_home 	0x02	//Return Home

//wait 37 us 
//these performs on data read/write
#define dec_cursor		0x04	//Decrement Cursor (No shift)
#define shift_right 	0x05	//Shift Display Right (Decrement Cursor)
#define inc_cursor 		0x06	//Increment Cursor (No shift)
#define shift_left 		0x07	//Shift Display Left (Increment Cursor)

#define disp_OFF 							0x08	//disp_off + cursor_off + cBlink_off
#define disp_ON								0x0C	//disp_on  + cursor_off + cBlink_off
#define disp_curs_ON 					0x0E	//disp_on  + cursor_on  + cBlink_off
#define disp_curs_cBlink_ON 	0x0F	//disp_on  + cursor_on  + cBlink_on

//content of address counter are not changed
#define shift_curs_left				0x10	//shift cursor left
#define shift_curs_right			0x14	//shift cursor right
#define shift_disp_curs_left	0x18	//shift display left, cursor follows
#define shift_disp_curs_right	0x1C	//shift display right, cursor follows


#define fourbit_oneline_8dot		0x20	//4bit bus mode 1 line disp 5x8 format
#define fourbit_oneline_11dot		0x24	//4bit bus mode 1 line disp 5x11 format
#define fourbit_twoline_8dot		0x28	//4bit bus mode 2 line disp 5x8 format
#define fourbit_twoline_11dot		0x2C	//4bit bus mode 2 line disp 5x11 format
#define eightbit_oneline_8dot		0x30	//8bit bus mode 1 line disp 5x8 format
#define eightbit_oneline_11dot	0x34	//8bit bus mode 1 line disp 5x11 format
#define eightbit_twoline_8dot		0x38	//8bit bus mode 2 line disp 5x8 format
#define eightbit_twoline_11dot	0x3C	//8bit bus mode 2 line disp 5x11 format


#define beg_firstLine	0x80	//Force Cursor to Beginning of first line
#define beg_secLine 	0xC0 	//Force Cursor to Beginning of second line


void GPIO_Init_LCD4bit(void);
void GPIO_Init_LCD8bit(void);
void LCD_Init_4bit(void);
void LCD_cmd_4bit(unsigned char command);
void LCD_data_4bit(unsigned char ch);
void LCD_string4bit(unsigned char *str);
void LCD_Init_8bit(void);
void LCD_cmd_8bit(unsigned char command);
void LCD_data_8bit(unsigned char ch);
void LCD_string8bit(unsigned char str[128]);

#endif //__LCD_LIB_H__
