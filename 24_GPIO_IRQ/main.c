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
	
////	//Unlock PJ0
//	if( (GPIOJ_AHB->LOCK) == 1){ //locked
//		GPIOJ_AHB->LOCK |= 0x4C4F434B; //for unlock	
//	}

//	__IO uint32_t *tmp;
//	tmp = (__IO uint32_t *) GPIOJ_AHB->CR;
//	*tmp = 0x01;
	GPIOJ_AHB->PUR |= (0x01); //enable pullup for SW1 and SW2 (PJ0, PJ1)
	GPIOJ_AHB->DIR &= ~(0x01); //input
	GPIOJ_AHB->DEN |= (0x01);
	
	SYSCTL->RCGCGPIO |= (1U<<5);
	GPIOF_AHB->DIR |= (LED4 | LED3); //output
	GPIOF_AHB->DEN |= (LED4 | LED3);
	

	__disable_irq(); //CPSR daki int bayragini kapatiyor.
	
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

	GPIOJ_AHB->IM &= ~(0x01); //Mask the corresponding port by clearing the IME field in the GPIOIM register. Interrupt'i kapattik
	GPIOJ_AHB->IS &= ~(0x01);	//make SW1 edge sensitive(0 edge, 1 level sens)
	GPIOJ_AHB->IBE &= ~(0x01); //Interrupt Both Edges (0 IEV karar versin demek)
	GPIOJ_AHB->IEV &= ~(0x01); //clearing is falling edge detection/low level (0 falling edge, 1 rising edge)
	GPIOJ_AHB->ICR |= (0x01); //clear any prior interrupt
	GPIOJ_AHB->IM |=  (0x01);	//unmask interrupt IRQ açtik
	
	
	NVIC->IP[51] = (0x03);	// set interrupt to priority 3
	NVIC->ISER[1] = (1U<<19);	//Enable IRQ51 32+19
	
	__enable_irq();
	
	while(1){
		
		//Toggle LED3
		GPIOF_AHB->DATA ^= LED3;
		delay(1000*mili);
		
	}
	
	
		
	
	return 0;
	
}


void GPIOJ_Handler(void){ //can fetch prototype 
	
	volatile int readback;
	
	
	//toogle green LED4 3 times 
	int i = 0;
	for(i = 0; i<3; i++){
		GPIOF_AHB->DATA |= LED4;
		delay(250*mili);
		GPIOF_AHB->DATA &= ~LED4;
		delay(250*mili);
		
	}
	
	GPIOJ_AHB->ICR |= (0x03); //clear any prior interrupt
	
	readback = GPIOJ_AHB->ICR;
	
	
}

void delay(unsigned int time){
	
	time = (unsigned int)time*1.33929;
	
	unsigned int j=0;
	SYSCTL->RCGCTIMER |= 0x01;
	for(int i = 0; i<1; i++){}
	
	TIMER0->CTL &= ~(1U<<0);
	TIMER0->CFG = 0x04;

	TIMER0->TAMR = 0x02;
	TIMER0->TAILR = 25-1;

	TIMER0->ICR =0x01;
	TIMER0->CTL |= (1U<<0);
		
	unsigned int temp = (TIMER0->RIS &0x1);
		
	for (j=0; j<time; j++){
		while(temp==0x00){
			temp = (TIMER0->RIS &0x1);
		}
		TIMER0->ICR = 0x01;
	}
	

}
	




