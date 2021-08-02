#include "keypad.h"
#include "TM4C129.h"                    // Device header
#include "delay.h"

const unsigned char keymap[4][4]=
{ 
	{ '1', '2', '3', 'A' },
	{ '4', '5', '6', 'B' },
	{ '7', '8', '9', 'C' },
	{ '*', '0', '#', 'D' }		
};

void GPIO_Init_KeyPad(void){
	
	unsigned int GPIO_LOCK_KEY = 0x4C4F434B;
	
	//pins A
	//0. bit port A 
	//0b 0000 0000 0000 0001
	SYSCTL->RCGCGPIO |= (1U<<0); 
	
	
	// 0b 0000 0000
	// 		Columns Rows
	
	//Rows will be output and open drain
	// Rows 0xAA 
	//0b 1010 1010  	
	// Colums will be input pull up reg opened
	// Colums 0x55 
	//0b 0101 0101 
	
//	GPIOA_AHB->LOCK = GPIO_LOCK_KEY; //gereksiz olabilir

	GPIOA_AHB->DIR	= 0xAA;
	GPIOA_AHB->ODR 	= 0xAA;
	GPIOA_AHB->PUR 	= 0x55;
//	GPIOA_AHB->AFSEL = 0x00;	//gereksiz olabilir
	GPIOA_AHB->DEN 	= 0xFF;

	
	//   C3  C2  C1  C0  R3  R2  R1  R0
	// ___8___7___6___5___4___3___2___1___		Keypad
	//   PA6 PA4 PA2 PA0 PA7 PA5 PA3 PA1
	
	//  
	//   PA7 PA6 PA5 PA4 PA3 PA2 PA1 PA0
	// ___7___6___5___4___3___2___1___0___		MCU
	//    R3  C3  R2  C2  R1  C1  R0  C0    
}



unsigned char getKey(void){
	
	unsigned char k_row, k_col;
	
	GPIOA_AHB->DATA &= ~(0xAA); //rows grounded (look up open drain) //clean first four bit

	k_col = (GPIOA_AHB->DATA&0x55 ); //cols are stable at High //0b 0101 0101
	
	if(k_col == 0x55){ //because pull up 
		//no key pressed
		return 0;
	}
	for(;;){
		
		GPIOA_AHB->DATA = ~(0xAA); //clean first four bit
		GPIOA_AHB->DATA |= (1U<<1); //0b 0000 00'1'0 //all grounded but first high zet
		delay_ms(5);
		if( (GPIOA_AHB->DATA&0x55 ) == 0x55){ // if it shows all High, then 0th row pressed
			k_row = 0;
			break;
		}
		
		GPIOA_AHB->DATA = ~(0xAA); //clean first four bit
		GPIOA_AHB->DATA |= (1U<<3); //0b 0000 '1'000 //all grounded but second high zet
		delay_ms(5);
		if( (GPIOA_AHB->DATA&0x55 ) == 0x55){
			k_row = 1;
			break;
		}
		
		GPIOA_AHB->DATA = ~(0xAA); //clean first four bit
		GPIOA_AHB->DATA |= (1U<<5); //0b 00'1'0 0000//all grounded but third high zet
		delay_ms(5);
		if( (GPIOA_AHB->DATA&0x55 ) == 0x55){
			k_row = 2;
			break;
		}
		
		GPIOA_AHB->DATA = ~(0xAA); //clean first four bit
		GPIOA_AHB->DATA |= (1U<<7); //0b '1'000 0000 //all grounded but fourth high zet
		delay_ms(5);
		if( (GPIOA_AHB->DATA&0x55 ) == 0x55){
			k_row = 3;
			break;
		}
		
		return 0;;
	}
	
	if( k_col == 0x54 ) //0th col //k_col & 0b 0000 0001 == 0b 0000 0000
		return (keymap[k_row][0]);
	if( k_col == 0x51) //1st col //k_col & 0b 0000 0100 == 0b 0000 0000
		return (keymap[k_row][1]);
	if( k_col == 0x45 ) //2nd col //k_col & 0b 0010 0000 == 0b 0000 0000 
		return (keymap[k_row][2]); 
	if( k_col == 0x15) //3th col //k_col & 0b 1000 0000 == 0b 0000 0000
		return (keymap[k_row][3]);
	
	return 0;
}

