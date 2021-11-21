#include "TM4C129.h"                    // Device header

#define LED4	(1U<<0)	//PF0
#define LED3	(1U<<4)	//PF4

#define micro 1U
#define mili 1000U
#define sec	1000000U


void delay(unsigned int time);

unsigned int state = 0;


int main(void){
	
	//Switch at PJ0 port PJ1
	SYSCTL->RCGCGPIO |= (1U<<8);
	while( (SYSCTL->PRGPIO &(1U<<8)) != (1U<<8)) {}; //Allow time to finish activating GPIO

	GPIOJ_AHB->PUR |= (0x03); //enable pullup for SW1 and SW2 (PJ0, PJ1)
	GPIOJ_AHB->DIR &= ~(0x03); //input
	GPIOJ_AHB->DEN |= (0x03);
	
	SYSCTL->RCGCGPIO |= (1U<<5);
	GPIOF_AHB->DIR |= (LED4 | LED3); //output
	GPIOF_AHB->DEN |= (LED4 | LED3);
	

	__disable_irq(); //CPSR daki int bayragini kapatiyor.

	GPIOJ_AHB->IM &= ~(0x03); //Mask the corresponding port by clearing the IME field in the GPIOIM register. Interrupt'i kapattik
	GPIOJ_AHB->IS &= ~(0x01);	//make SW1 edge sensitive(0 edge, 1 level sens)
	GPIOJ_AHB->IBE &= ~(0x01); //Interrupt Both Edges (0 IEV karar versin demek)
	GPIOJ_AHB->IEV &= ~(0x01); //clearing is falling edge detection/low level (0 falling edge, 1 rising edge)
	GPIOJ_AHB->ICR |= (0x03); //clear any prior interrupt
	GPIOJ_AHB->IM |=  (0x03);	//unmask interrupt IRQ açtik
	
	NVIC->IP[51] = (0x03);	// set interrupt to priority 3
	NVIC->ISER[1] = (1U<<19);	//Enable IRQ51 32+19
	
	__enable_irq();
	
	while(1){
		
	}
	
	return 0;
	
}


void GPIOJ_Handler(void){ //can fetch prototype 
	
	volatile int readback;
	//toogle green LED4 3 times 
	int i = 0;
	
//GPIOMIS	
//If a bit is set in this register, the corresponding 
//interrupt has triggered an interrupt to the interrupt controller.
	while(GPIOJ_AHB->MIS != 0){
		if(GPIOJ_AHB->MIS & 0x02){ // check if SW2(PJ1)
			
			for(i = 0; i<3; i++){
				GPIOF_AHB->DATA |= LED4;
				delay(250*mili);
				GPIOF_AHB->DATA &= ~LED4;
				delay(250*mili);	
			}

			GPIOJ_AHB->ICR |= (0x03); //clear any prior interrupt
			readback = GPIOJ_AHB->ICR;
			
		}else if(GPIOJ_AHB->MIS & 0x01){ // check if SW1(PJ0)
			
			for(i = 0; i<3; i++){
				GPIOF_AHB->DATA |= LED3;
				delay(250*mili);
				GPIOF_AHB->DATA &= ~LED3;
				delay(250*mili);	
			}

			GPIOJ_AHB->ICR |= (0x03); //clear any prior interrupt
			readback = GPIOJ_AHB->ICR;
			
		}else{
			
			GPIOJ_AHB->ICR |= (0x03); //clear any prior interrupt
			readback = GPIOJ_AHB->ICR;

		}
	}
	
}

void delay(unsigned int time){

	unsigned int j=0;
	SYSCTL->RCGCTIMER |= 0x01;
	while( (SYSCTL->PRTIMER &(1U<<0)) != (1U<<0)) {}; //Allow time to finish activating 
	
	TIMER0->CTL &= ~(1U<<0);
	TIMER0->CFG = 0x04;

	TIMER0->TAMR = 0x02;
	TIMER0->TAILR = 16-1;

	TIMER0->ICR =0x01;
	TIMER0->CTL |= (1U<<0);
		
		
	for (j=0; j<time; j++){
		while((TIMER0->RIS &0x1)==0x00){	};
		TIMER0->ICR = 0x01;
	}
	

}



