#include "TM4C129.h"                    // Device header
#include "delay.h"

#define LED1 (1U<<1)	//PN1
#define LED2 (1U<<0)	//PN0
#define LED3 (1U<<4)	//PF4
#define LED4 (1U<<0)	//PF0




int main(){
	SYSCTL->RCGCGPIO |= 0x1020U;	//N port => 12.bit F portu => 5. bit
															//0001 0000 0010 0000

	GPIOF_AHB->DIR |= (LED3 | LED4);	//Direction output 
	GPION->DIR |= (LED1 | LED2);
	
	GPIOF_AHB->DEN |= (LED3 | LED4);
	GPION->DEN |= (LED1 | LED2);
	
	GPIOF_AHB->DATA = GPIOF_AHB->DATA | (LED3 | LED4);
	
	while(1){
		
		GPION->DATA = GPION->DATA | (LED1);
		wait(500000);

		GPION->DATA = GPION->DATA & ~((LED1));
		GPION->DATA = GPION->DATA | (LED2);
		wait(500000);

		GPION->DATA = GPION->DATA & ~((LED2));
		GPIOF_AHB->DATA = GPIOF_AHB->DATA | (LED3);
		wait(500000);

		GPIOF_AHB->DATA = GPIOF_AHB->DATA & ~((LED3));
		GPIOF_AHB->DATA = GPIOF_AHB->DATA | (LED4);
		wait(500000);

		GPIOF_AHB->DATA = GPIOF_AHB->DATA & ~((LED4));
		GPIOF_AHB->DATA = GPIOF_AHB->DATA | (LED3);
		wait(500000);
		
		GPIOF_AHB->DATA = GPIOF_AHB->DATA & ~((LED3));
		GPION->DATA = GPION->DATA | (LED2);
		wait(500000);
		
		GPION->DATA = GPION->DATA & ~((LED2));
		GPION->DATA = GPION->DATA | (LED1);
		wait(500000);
		
		GPION->DATA = GPION->DATA & ~((LED1));
		wait(500000);
	}
	
	
	return 0;
}


