#include "GPIO_Init.h"

#include "TM4C129.h"                    // Device header
#include "delay.h"


void onBoardLED_start(void){
	
	//Direction 
	//LEDs->output
	GPIOF_AHB->DIR |= (LED3 | LED4);	 
	GPION->DIR |= (LED1 | LED2);
	//Digital Enable
	//LED
	GPIOF_AHB->DEN |= (LED3 | LED4); 
	GPION->DEN |= (LED1 | LED2);
	
}


void onBoardSW_start(void){
	
	//Direction
	//SWs->input
	GPIOJ_AHB->LOCK = KEY;
	//GPIOJ_AHB->CR |= 0xFF;
	GPIOJ_AHB->PUR |= (SW1 | SW2);
	GPIOJ_AHB->DIR &= ~(SW1 | SW2);
	
	//Digital Enable
	//sw
	GPIOJ_AHB->DEN |= (SW1 | SW2);
}


void SYSCTRL_LED_SW_start(void){
	
	// LED						LED							SWITCH
	//N_Portu=>12.bit F_Portu=>5.bit J_Portu=>8.bit
	//0001 0001 0010 0000
	SYSCTL->RCGCGPIO |= 0x1120U;	
	
	
}
void GPIO_start(void){
	
	SYSCTRL_LED_SW_start();
	onBoardLED_start();
	onBoardSW_start();
}



uint32_t SW_INPUT(void){

	return (GPIOJ_AHB->DATA & (SW1 | SW2));
}

void LED_On(short int LED){
	
	if(LED == 1)
		GPION->DATA |= LED1;
	else if	(LED == 2)
		GPION->DATA |= LED2;
	else if (LED == 3)
		GPIOF_AHB->DATA |= LED3;
	else 
		GPIOF_AHB->DATA |= LED4;
	
}

void LED_Off(short int LED){
	if(LED == 1)
		GPION->DATA &= ~LED1;
	else if	(LED == 2)
		GPION->DATA &= ~LED2;
	else if (LED == 3)
		GPIOF_AHB->DATA &= ~LED3;
	else 
		GPIOF_AHB->DATA &= ~LED4;
	
}

void Blink(short int LED){
	LED_On(LED);
	delay(1*ns2S);
	LED_Off(LED);
	delay(1*ns2S);
}
