#include "TM4C129.h"                    // Device header

#define LED1 (1U<<1)	//PN1
#define LED2 (1U<<0)	//PN0
#define LED3 (1U<<4)	//PF4
#define LED4 (1U<<0)	//PF0

#define SW1  (1U<<0)	//PJ0
#define SW2  (1U<<1)	//PJ1

#define KEY	0x4C4F434B

#define ns2S 1E6

uint32_t SW_INPUT(void);
void delay(int time);
void LED_On(short int LED);
void LED_Off(short int LED);
void Blink(short int LED);

const uint32_t sw1 = 0x01;
const uint32_t sw2 = 0x02;

uint32_t sw_state;


int main(){
	
	SYSCTL->RCGCGPIO |= 0x1120U;	//N_Portu=>12.bit F_Portu=>5.bit J_Portu=>8.bit
															//0001 0001 0010 0000

	//LEDs->output
	GPIOF_AHB->DIR |= (LED3 | LED4);	//Direction output 
	GPION->DIR |= (LED1 | LED2);
	//SWs->input
	GPIOJ_AHB->LOCK = KEY;
	//GPIOJ_AHB->CR |= 0xFF;
	GPIOJ_AHB->PUR |= (SW1 | SW2);
	GPIOJ_AHB->DIR &= ~(SW1 | SW2);
	
	//LED
	GPIOF_AHB->DEN |= (LED3 | LED4); //Digital Enable
	GPION->DEN |= (LED1 | LED2);
	//sw
	GPIOJ_AHB->DEN |= (SW1 | SW2);
	
	while(1){
		sw_state = SW_INPUT();
		
		switch(sw_state){			
			case sw1:
				Blink(1);
			break;
			case sw2:
				Blink(2);
			break;
			case (sw1 + sw2):
				Blink(3);
			break;
			default:
				Blink(4);
			break;
			
		}
		
	}

	
	return 0;
}


uint32_t SW_INPUT(void){

	return (GPIOJ_AHB->DATA & (SW1 | SW2));
}

void delay(int time){
	int volatile delay=0;
	while(delay<time)
		++delay;
	
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




