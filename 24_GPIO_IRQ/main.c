#include "TM4C129.h"                    // Device header


#define LED4	(1U<<0)	//PF0



void delayMs(int delay);



int main(void){
	
	//Switch at PJ0 port PJ1
	SYSCTL->RCGCGPIO |= (1U<<8);
	
//	//Unlock PJ0
//	GPIOJ_AHB->LOCK |= 0x4C4F434B;
//	GPIOJ_AHB->CR		=	0x01;
//	GPIOJ_AHB->LOCK	= 0;

	GPIOJ_AHB->DIR &= ~(0x03); //input
	GPIOJ_AHB->DEN |= (0x03);
	GPIOJ_AHB->PUR |= (0x03); //enable pullup for SW1 and SW2 (PJ0, PJ1)
	
	SYSCTL->RCGCGPIO |= (1U<<8);
	GPIOF_AHB->DIR |= LED4; //output
	GPIOF_AHB->DEN |= LED4;
	

	__disable_irq();
	
// All bits are cleared by a reset
	
//	To prevent false interrupts, the following steps should be taken when re-configuring GPIO
//	edge and interrupt sense registers:
//	1. Mask the corresponding port by clearing the IME field in the GPIOIM register.
//	2. Configure the IS field in the GPIOIS register and the IBE field in the GPIOIBE register.
//	3. Clear the GPIORIS register.
//	4. Unmask the port by setting the IME field in the GPIOIM register
	
//	GPIOJ_AHB->IM &= ~(0x03); 
//	GPIOJ_AHB->IS &= ~(0x03);
//	GPIOJ_AHB->IBE &= ~(0x03);
//	GPIOJ_AHB->RIS &= ~(0x03);
// 	For a GPIO edge-detect interrupt, the RIS bit in the 
// GPIORIS register is cleared by writing a ‘1’ to the 
// corresponding bit in the GPIO Interrupt Clear (GPIOICR) register
//	GPIOA_AHB->IM |= (0x03); 

	GPIOJ_AHB->IS &= ~(0x03);	//make SW1 and SW2 edge sensitive
	GPIOJ_AHB->IBE &= ~(0x03);
	GPIOJ_AHB->IEV &= ~(0x03); //clearing is falling edge detection/low level
	GPIOJ_AHB->ICR |= (0x03); //clear any prior interrupt
	GPIOJ_AHB->IM |=  (0x03);	//unmask interrupt
	
	
	NVIC->IP[30] = (3U<<5);	// set interrupt to priority 3
	NVIC->ISER[0] = 0x40000000;	//Enable IRQ30
	
	__enable_irq();
	
	while(1){
		
		//Toggle LED4
		GPIOF_AHB->DATA |= ~LED4;
		delayMs(100);
		GPIOF_AHB->DATA &= ~LED4;
		delayMs(100);
		
	}
	
	
		
	
	return 0;
	
}


void GPIOF_Handler(void){ //can fetch prototype 
	
	volatile int readback;
	
	
	//toogle green LED4 3 times 
	int i = 0;
	for(i = 0; i<3; i++){
		GPIOF_AHB->DATA |= LED4;
		delayMs(100);
		GPIOF_AHB->DATA &= ~LED4;
		delayMs(100);
		
	}
	
	GPIOJ_AHB->ICR |= (0x03); //clear any prior interrupt
	
	readback = GPIOJ_AHB->ICR;
	
	
}


void delayMs(int delay){
	int i=0;
	for(i=0; i<delay; i++){
		int j=0;
		for(j=0; j<3180; j++){
			
		}
		
	}
	
}





