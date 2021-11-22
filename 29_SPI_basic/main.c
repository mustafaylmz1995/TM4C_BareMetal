#include "TM4C129.h"                    // Device header


int main(){
	
	
	while(1){
		
		
		
	}
	
	
}


void SSI_init(void){
	
	SYSCTL->RCGCSSI |= (1U<<2); //SSI2 will be used
	while( (SYSCTL->PRSSI & (1U<<2)) != (1U<<2) ) {}; //whether the SSI modules are ready to be accessed by software
	SYSCTL->RCGCGPIO |= (1U<<3); //PORTD opened for SSI signals
	while( (SYSCTL->PRGPIO &(1U<<3)) != (1U<<3) ) {}; //Allow time to finish activating GPIOD
	SYSCTL->RCGCGPIO |= (1U<<5); //PORTF opened for LED
	while( (SYSCTL->PRGPIO &(1U<<5)) != (1U<<5) ) {}; //Allow time to finish activating GPIOF
	
	GPIOD_AHB->AMSEL &= ~((1U<<1) | (1U<<3)); //Disable analog for PD3 SS2CLK and PD0 SSI2XDAT0(SSI2Tx)
	GPIOD_AHB->DEN |= ((1U<<1) | (1U<<3)); // PD3 SS2CLK and PD0 SSI2XDAT0(SSI2Tx) Digitally enable
	
	GPIOD_AHB->AFSEL |= ((1U<<1) | (1U<<3)); //Alternate Function
	GPIOD_AHB->PCTL &= ~((1U<<1) | (1U<<3)); //configure PD3 and PD0
	GPIOD_AHB->PCTL |= ((0xF<<0) | (0xF<<3));	//15th Functionality is SSI2 
		
	GPIOF_AHB->DEN |= (1U<<0); //PF0 LED4 opened for digital enable
	GPIOF_AHB->DIR |= (1U<<0); //PF0 LED4 sets as Digital Output
	GPIOF_AHB->DATA|= (1U<<0); //PF0 LED4 sets as HIGH
		
	/*
	*		SPI Master, CPOL = 0, CPHA = 0, Clk = 4 MHz, 16 bit data
	*/
		
		
}








