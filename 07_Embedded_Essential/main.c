#include <stdio.h>
#include <stdint.h>
#include "TM4C129.h"                    // Device header


#define LED1 (1U<<1)	//PN1
#define LED2 (1U<<0)	//PN0
#define LED3 (1U<<4)	//PF4
#define LED4 (1U<<0)	//PF0


void wait(unsigned int time){
	
	uint32_t volatile i = 0;
	while(i<time)
		i++;
	
}


int main(){
	
	SYSCTL->RCGCGPIO |= 0x1020U;	//N port => 12.bit F portu => 5. bit
															//0001 0000 0010 0000

	GPIOF_AHB->DIR |= (LED3 | LED4);	//Direction output 
	GPION->DIR |= (LED1 | LED2);
	
	GPIOF_AHB->DEN |= (LED3 | LED4);
	GPION->DEN |= (LED1 | LED2);
	
	GPIOF_AHB->DATA = GPIOF_AHB->DATA | (LED3 | LED4);
	
	while(1){
		
		GPION->DATA = GPION->DATA | (LED1 | LED2);
		wait(5000000);

		//GPIOF_AHB->DATA = GPIOF_AHB->DATA & ~((LED3 | LED4));
		GPION->DATA = GPION->DATA & ~((LED1 | LED2));
		wait(5000000);
	}
	
	return 0;
}


