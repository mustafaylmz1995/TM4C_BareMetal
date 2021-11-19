#include "TM4C129.h"                    // Device header

#define LED1 (1U<<1)	//PN1
#define LED2 (1U<<0)	//PN0
#define LED3	(1U<<4)	//PF4
#define LED4	(1U<<0)	//PF0

#define US2S 	1000000U
#define US2MS	1000U

void timer0A_1Hz_init(void);
void timer1A_10Hz_init(void);
void delay(unsigned int time);


int main(void){
	
	__disable_irq();
	
	SYSCTL->RCGCGPIO |= (1U<<5);
	GPIOF_AHB->DIR |= (LED4 | LED3); //output
	GPIOF_AHB->DEN |= (LED4 | LED3);
	
	SYSCTL->RCGCGPIO |= (1U<<12);
	GPION->DIR |= (LED1 | LED2); //output
	GPION->DEN |= (LED1 | LED2);

	
	timer0A_1Hz_init();
	timer1A_10Hz_init();
//	
	__enable_irq();
	
	
	while(1){
		GPION->DATA^= LED1;
		delay(500*US2MS);

	}
	
	
	
}

void timer0A_1Hz_init(void){
	
	SYSCTL->RCGCTIMER |= 0x01;	//enable clk to Timer Block 0
	for(int i = 0; i<1; i++){}
		
	TIMER0->CTL &= ~(1U<<0);	//disable Timer0 before initialization
	TIMER0->CFG = 0x04;	//16-bit option
	TIMER0->TAMR = 0x02;	//Periodic Mode and Down-Counter
	
	//periodic down count modes, this register acts as a 
	//true prescaler for the timer counter
	TIMER0->TAPR = 25*US2S/500; //we want to 1sec 65535 den daha fazla 
	//sayamadigi icin daha kücük sayabilecegi bir deger elde etmeye calistik 25M/500=50k 
	TIMER0->TAILR = 50000;//used to load the starting count value
	TIMER0->ICR = 0x01; //clearing interupt controller register

	TIMER0->IMR |= 0x01; //IRQ Mask Reg, Timer0A timeout interrupt enabled
	TIMER0->CTL |= 0x01; //enable Timer0A after initialization
	
	NVIC->ISER[0]=0x00080000; //enable IRQ19 for Timer0A on Vector Table 
		
}

void timer1A_10Hz_init(void){
	
	SYSCTL->RCGCTIMER |= 0x02;	//enable clk to Timer Block 1
	for(int i = 0; i<1; i++){}
		
	TIMER1->CTL &= ~(1U<<0);	//disable Timer1 before initialization
	TIMER1->CFG = 0x04;	//16-bit option
	TIMER1->TAMR = 0x02;	//Periodic Mode and Down-Counter
	
	//periodic down count modes, this register acts as a 
	//true prescaler for the timer counter
	TIMER1->TAPR = 25*US2S/500; //we want to 1sec 65535 den daha fazla 
	//sayamadigi icin daha kücük sayabilecegi bir deger elde etmeye calistik 25M/500=50k 
	TIMER1->TAILR = 5000;//used to load the starting count value //10 Hz
	TIMER1->ICR = 0x01; //clearing interupt controller register

	TIMER1->IMR |= 0x01; //IRQ Mask Reg, Timer1A timeout interrupt enabled
	TIMER1->CTL |= 0x01; //enable Timer1A after initialization
	
	NVIC->ISER[0]=0x00200000; //enable IRQ21 for Timer1A on Vector Table 
		
}

void TIMER0A_Handler(){
	
		volatile int clear;
	
	 if(TIMER0->MIS & 0x01){ //timeout occurs
		
		 GPIOF_AHB->DATA ^= LED3;
		 TIMER0->ICR = 0x01;
		 clear = TIMER0->ICR; //a read to force clearing if irq flag
		
	}else{
		TIMER0->ICR &= ~(0x01);
		clear = TIMER0->ICR;
	}
	
}

void TIMER1A_Handler(){
	
		volatile int clear;
	
	 if(TIMER1->MIS & 0x01){ //timeout occurs
		
		 GPIOF_AHB->DATA ^= LED4;
		 TIMER1->ICR = 0x01;
		 clear = TIMER1->ICR; //a read to force clearing if irq flag
		
	}else{
		TIMER1->ICR &= ~(0x01);
		clear = TIMER1->ICR;
	}
	
}




void delay(unsigned int time){
	
	time = (unsigned int)time*1.33929;
	
	unsigned int j=0;
	SYSCTL->RCGCTIMER |= 0x04; //TIMER2A
	for(int i = 0; i<1; i++){}
	
	TIMER2->CTL &= ~(1U<<0);
	TIMER2->CFG = 0x04;

	TIMER2->TAMR = 0x02;
	TIMER2->TAILR = 25-1;

	TIMER2->ICR =0x01;
	TIMER2->CTL |= (1U<<0);
		
	unsigned int temp = (TIMER2->RIS &0x1);
		
	for (j=0; j<time; j++){
		while(temp==0x00){
			temp = (TIMER2->RIS &0x1);
		}
		TIMER2->ICR = 0x01;
	}
	

}





