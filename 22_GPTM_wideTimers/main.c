#include "TM4C123.h"                    // Device header

//tm4c123G 

#define BLUE (1U<<2)
void wtimer1A_delaySec(int sec);

int main(){
	
	SYSCTL->RCGCGPIO |= (1U<<5);
	GPIOF->DIR = BLUE;
	GPIOF->DEN = BLUE;
	
	
	while(1){
		
		GPIOF->DATA ^= BLUE;
		wtimer1A_delaySec(10);
		
		
	}

	
	
	return 0;
}


void wtimer1A_delaySec(int sec){
	
	int s;
	
	SYSCTL->RCGCWTIMER |= 0x02; //enable clk to wTimer Module 1
	WTIMER1->CTL = 0;	// disable Wtimer before init.
	WTIMER1->CFG = 0x04;	// 32-bit option
	WTIMER1->TAMR = 0x02;	// periodic mode and down cnt
	WTIMER1->TAILR = 16000000-1;
	WTIMER1->ICR = 1;
	WTIMER1->CTL = 0x01;	//enable Wtimer A after init.
	
	for(s =0; s<sec; s++){
		
		while( (WTIMER1->RIS & 0x01) == 0); //wait for timeout
		WTIMER1->ICR = 0x1; //clear the TimerA timeour flag
		
		
	}
	
}
