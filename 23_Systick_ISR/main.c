#include "TM4C129.h"                    // Device header


#define LED4	(1U<<0)	//PF0

int main(void){
	
	
	SYSCTL->RCGCGPIO |= (1U<<5);
	GPIOF_AHB->DIR |= LED4;
	GPIOF_AHB->DEN |= LED4;
	
	// Configure systick
	
	__disable_irq();
	SysTick->LOAD = 16000000-1;
	SysTick->CTRL = 7; //Enable Systick, Use interrupt, use sys clk
	
	__enable_irq();
	
	
	
	while(1){
			
		
	}
	
	
	return 0;
}


void SysTick_Handler(void){ //no need to prototype
	
	//perform dsp/irq/next task/scheduler
	
	GPIOF_AHB->DATA ^= LED4;
	
}





