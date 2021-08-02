#include "TM4C129.h"                    // Device header
#include "lcd_lib.h"
#include "delay.h"
#include "keypad.h"



int main(){
	
	unsigned char key;
	int temp =0;
	LCD_Init_4bit();
	delay_ms(100);
	
	
	GPIO_Init_KeyPad();


	//2. SYSTICK TIMER örnegi yap
	//3. GTIMER orneklerin ypa
	
	
	while(1){
		key = getKey();
		delay_ms(20);
		if(key != 0){
			LCD_data_4bit(key);
			temp++;
			if( (temp != 0) && (temp%16 == 0) ){
				LCD_cmd_4bit(beg_secLine);
			}
			if( (temp != 0) && (temp%32 == 0)){
				LCD_cmd_4bit(clc_disp);	//Clear Display
				LCD_cmd_4bit(beg_firstLine);
				temp=0;
			}
		}
	}
	
	return 0;
}


