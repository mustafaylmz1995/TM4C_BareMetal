
#include "delay.h"

#include "TM4C129.h"                    // Device header

void delay(unsigned int time){
	
	unsigned int j=0;
	SYSCTL->RCGCTIMER |= 0x04; //TIMER2A
	while( (SYSCTL->PRTIMER &(1U<<2)) != (1U<<2)) {}; //Allow time to finish activating 
	
	TIMER2->CTL &= ~(1U<<0);
	TIMER2->CFG = 0x04;

	TIMER2->TAMR = 0x02;
	TIMER2->TAILR = 16-1;

	TIMER2->ICR =0x01;
	TIMER2->CTL |= (1U<<0);
		
	for (j=0; j<time; j++){
		while((TIMER2->RIS &0x1) == 0x00){	};
		TIMER2->ICR = 0x01;
	}
	

}

