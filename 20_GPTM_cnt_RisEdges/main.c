#include "TM4C129.h"                    // Device header

void Timer4A_RisingEdgeEvent_init(void);
unsigned int Timer4A_RisingEdgeEvent_capture(void);

unsigned int number_of_events = 0;

int main(void){
	
	Timer4A_RisingEdgeEvent_init();
	
	while(1){
		number_of_events = Timer4A_RisingEdgeEvent_capture();
	}
	
	return 0;
}


void Timer4A_RisingEdgeEvent_init(void){
	
	SYSCTL->RCGCTIMER |= 0x10; //enable clk to Timer Block 4 0b 0001 0000
	SYSCTL->RCGCGPIO	|= 0x02; //Enable clk to PORTB
	
	GPIOB_AHB->DIR	&= ~(1U<<0);	//make PB0 an input pin
	GPIOB_AHB->DEN	|= (1U<<0);	//make PB0 a digital pin
	GPIOB_AHB->AFSEL|= (1U<<0);	//enable alternate function on PB0
	//0th bit 
	GPIOB_AHB->PCTL &= ~(0x0000000F); //configure PB0 as T4CCP0 pin
	GPIOB_AHB->PCTL |= 0x00000003; //Third Functionality is Timerblock
	

	//Input Edge Count Mode
	
	// TAEN 0. bit and TBEN 8. bit
	TIMER4->CTL  = (0U<<0); //disable TIMER4A
	
	//For a 16/32-bit timer, this value selects the 16-bit timer configuration.
	TIMER4->CFG = 0x4; 
	
	//Write TACDIR to 0x1 for timer starts from value of 0x00 (4th bit)
	//Write TACMR to 0x0 for Edge Count mode (2nd bit)
	//Write a value of 0x03 for Capture mode (0th and 1st bits)
	TIMER4->TAMR = 0x13; //up-count, edge-count, capture mode
	
	TIMER4->TAMATCHR = 0xFFFF;	//set the count limit, compared to TAR to determine match evetn
	TIMER4->TAPMR = 0xFF; // used with TAMATCHR to expand to 0xFFFF FF with prescaler
	
	TIMER4->CTL |= (1U<<0);
	
}

unsigned int Timer4A_RisingEdgeEvent_capture(void){
	
	return TIMER4->TAR; // compared with TAMATCHR to determine match event
	
}




