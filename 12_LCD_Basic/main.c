#include "TM4C129.h"                    // Device header

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


void delay_ms(unsigned int ms){
	unsigned int s = ms*1800;
	while(s>0){
		s--;
	}
}
void delay_us(unsigned int us){
	unsigned int s = us*10;
	while(s>0){
		s--;
	}
}

void GPIO_Init(void){
	
	//Data pins M, Control Pins N
	//11. bit port M, 12. bit port N
	//0b 0001 1000 0000 0000
	SYSCTL->RCGCGPIO |= (1U<<11); 
	SYSCTL->RCGCGPIO |= (1U<<12); 	

	//PN0 Register Select
	//PN1 Read/Write
	//PN2 Enable	
	//0b 0000 0111
	GPION->DIR	|= (Reg_Select | Read_Write | Enable);
	//Control Pins Enabled
	GPION->DEN 	|= (Reg_Select | Read_Write | Enable);
	
	//All PM pins output
	GPIOM->DIR	|= 0xFF;
	//Data Pins Enabled
	GPIOM->DEN 	|= 0xFF;		
	
}
	
void LCD_cmd(unsigned char command){
	
	GPION->DATA &= ~((Reg_Select + Read_Write + Enable));
	GPIOM->DATA = command;
	delay_ms(500);
	//To secure command
	//Enable 1 others 0 send a pulse
	GPION->DATA |= Enable;
	delay_ms(1000);
	GPION->DATA &= ~(Enable);
	
	if(command<4){
		delay_ms(5000);
	}else{
		delay_ms(1000);
	}
		
}

void LCD_Init(void){
	
	GPIO_Init();
	
	delay_ms(500);
	
	LCD_cmd(eightbit_twoline_8dot);	//8 bit, 2 line, 5x8 font
	LCD_cmd(inc_cursor);	//shift cursor right automatically
	LCD_cmd(disp_curs_cBlink_ON);	//Turn on Display Cursor and Cursor Blink
	LCD_cmd(clc_disp);	//Clear Display

}

void LCD_data(unsigned char ch){
	
	GPION->DATA &= ~((Reg_Select + Read_Write + Enable));
	GPION->DATA |= (Reg_Select);
	delay_ms(500);
	GPIOM->DATA = ch;
	delay_ms(500);
	GPION->DATA |= Enable;
	delay_ms(1000);
	GPION->DATA &= ~(Enable);
	delay_ms(1000);
	
}

int main(){

	LCD_Init();
	
	
	//-----------------------------------------------------------------
	LCD_data('y');
	LCD_data('o');
	LCD_data('u');
	LCD_data('t');
	LCD_data('u');
	LCD_data('b');
	LCD_data('e');
	LCD_data('.');
	LCD_data('c');
	LCD_data('o');
	LCD_data('m');
	
	LCD_cmd(beg_secLine);
	
	LCD_data('m');
	LCD_data('u');
	LCD_data('s');
	LCD_data('t');
	LCD_data('a');
	LCD_data('f');
	LCD_data('a');



	//-----------------------------------------------------------------
	
	
	return 0;
}




















