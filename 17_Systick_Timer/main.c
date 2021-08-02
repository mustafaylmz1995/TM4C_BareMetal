#include "TM4C129.h"                    // Device header
#include "delay.h"
#include "systick_lib.h"


#define LED1 (1U<<1)	//PN1
#define LED2 (1U<<0)	//PN0


int main(){
	
	SYSCTL->RCGCGPIO |= (1U<<12); //N_Portu=>12.bit
	//LEDs->output
	GPION->DIR |= (LED1 | LED2);
	//Digital Enable
	GPION->DEN |= (LED1 | LED2);
	
	SysTick_Init(1*US2S);
	
	while(1){
		// COUNT (16th bit)  0--> The SysTick timer has not counted to 0 since the last time this bit was read.
		// 									 1--> The SysTick timer has counted to 0 since the last time this bit was read.
		if(SysTick_Timeout()) {		
			GPION->DATA ^= (LED1 | LED2);		
		}
	}	
}




