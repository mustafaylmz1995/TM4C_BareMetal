#include "lcd_lib.h"
#include "delay.h"
#include "TM4C129.h"                    // Device header


void GPIO_Init_LCD4bit(void){
	
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
	
	//All PM pins output 4 bit mode 0xF0
	GPIOM->DIR	|= 0xF0;
	//Data Pins Enabled 4 bit mode 0xF0
	GPIOM->DEN 	|= 0xF0;		
	
	GPION->DATA = 0x00;
	GPIOM->DATA = 0x00;
	delay_ms(100);
	GPION->DATA = 0x0F;
	GPIOM->DATA = 0xFF;
	delay_ms(100);
	GPION->DATA = 0x00;
	GPIOM->DATA = 0x00;	
}

void LCD_Init_4bit(void){
	
	GPIO_Init_LCD4bit();

	delay_ms(100);
	

	LCD_cmd_4bit(fourbit_twoline_8dot);	//8 bit, 2 line, 5x8 font
	LCD_cmd_4bit(clc_disp);	//Clear Display	
	LCD_cmd_4bit(inc_cursor);	//shift cursor right automatically
	LCD_cmd_4bit(disp_curs_cBlink_ON);	//Turn on Display Cursor and Cursor Blink
	LCD_cmd_4bit(clc_disp);	//Clear Display
	LCD_cmd_4bit(beg_firstLine);
}

void LCD_cmd_4bit(unsigned char command){
	
	GPION->DATA &= ~((Reg_Select + Read_Write + Enable));
	GPIOM->DATA = command & 0xF0;
	delay_ms(10);
	//To secure command
	//Enable 1 others 0 send a pulse
	GPION->DATA |= Enable;
	delay_ms(30);
	GPION->DATA &= ~(Enable);
	
	if(command<4){
		delay_ms(30);
	}else{
		delay_ms(30);
	}
	
	GPIOM->DATA = (command & 0x0F)<<4;
	delay_ms(10);
	//To secure command
	//Enable 1 others 0 send a pulse
	GPION->DATA |= Enable;
	delay_ms(30);
	GPION->DATA &= ~(Enable);
	
	if(command<4){
		delay_ms(30);
	}else{
		delay_ms(30);
	}
		
}

void LCD_data_4bit(unsigned char ch){
	GPION->DATA &= ~((Reg_Select + Read_Write + Enable));
	GPION->DATA |= (Reg_Select);
	delay_ms(2);
	GPIOM->DATA = ch & 0xF0;
	delay_ms(10);
	GPION->DATA |= Enable;
	delay_ms(30);
	GPION->DATA &= ~(Enable);
	delay_ms(30);
	
	GPIOM->DATA = (ch & 0x0F)<<4;
	delay_ms(10);
	GPION->DATA |= Enable;
	delay_ms(30);
	GPION->DATA &= ~(Enable);
	delay_ms(30);
	
}

void LCD_string4bit(unsigned char *str){
	
	LCD_cmd_4bit(clc_disp);	//Clear Display
	LCD_cmd_4bit(beg_firstLine);
	
	for(int i = 0; i<128; i++){
		if( (i != 0) && (i%16 == 0) ){
			LCD_cmd_4bit(beg_secLine);
		}
		if( (i != 0) && (i%32 == 0)){
			LCD_cmd_4bit(clc_disp);	//Clear Display
			LCD_cmd_4bit(beg_firstLine);
		}
		if(*(str+i) == '\0'){
			break;
		}
		if(*(str+i) == '\n'){
			LCD_cmd_4bit(beg_secLine);
		}
		
		LCD_data_4bit(*(str+i));
		
	}
	
}

void GPIO_Init_LCD8bit(void){
	
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
	
	GPION->DATA = 0x00;
	GPIOM->DATA = 0x00;

}

void LCD_Init_8bit(void){
	
	GPIO_Init_LCD8bit();
	
	delay_ms(100);
	
	LCD_cmd_8bit(eightbit_twoline_8dot);	//8 bit, 2 line, 5x8 font
	LCD_cmd_8bit(clc_disp);	//Clear Display
	LCD_cmd_8bit(inc_cursor);	//shift cursor right automatically
	LCD_cmd_8bit(disp_curs_cBlink_ON);	//Turn on Display Cursor and Cursor Blink
	LCD_cmd_8bit(clc_disp);	//Clear Display
	LCD_cmd_8bit(beg_firstLine);
	
}

void LCD_cmd_8bit(unsigned char command){
	GPION->DATA &= ~((Reg_Select + Read_Write + Enable));
	GPIOM->DATA = command;
	delay_ms(10);
	//To secure command
	//Enable 1 others 0 send a pulse
	GPION->DATA |= Enable;
	delay_ms(30);
	GPION->DATA &= ~(Enable);

	if(command<4){
		delay_ms(30);
	}else{
		delay_ms(30);
	}
}
	
void LCD_data_8bit(unsigned char ch){
	
	GPION->DATA &= ~((Reg_Select + Read_Write + Enable));
	GPION->DATA |= (Reg_Select);
	delay_ms(2);
	GPIOM->DATA = ch;
	delay_ms(10);
	GPION->DATA |= Enable;
	delay_ms(30);
	GPION->DATA &= ~(Enable);
	delay_ms(30);
	
}

void LCD_string8bit(unsigned char *str){
	
	LCD_cmd_8bit(clc_disp);	//Clear Display
	LCD_cmd_8bit(beg_firstLine);
	
	for(int i = 0; i<128; i++){
		if( (i != 0) && (i%16 == 0) ){
			LCD_cmd_8bit(beg_secLine);
		}
		if( (i != 0) && (i%32 == 0)){
			LCD_cmd_8bit(clc_disp);	//Clear Display
			LCD_cmd_8bit(beg_firstLine);
		}
		if(*(str+i) == '\0'){
			break;
		}
		if(*(str+i) == '\n'){
			LCD_cmd_8bit(beg_secLine);
		}
		
		LCD_data_8bit(*(str+i));
		
	}
	
}




