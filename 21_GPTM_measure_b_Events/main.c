#include "TM4C129.h"                    // Device header


// Pinout For Ultrasonic HC-SR04
// Vcc  +5VDC
// Trig Digital Output Pin (PQ0)
// Echo Digital Input Pin (Put a 200K resistor) [ PB0 / T2CCP0 ]
// Gnd  Ground 

// Specifications
// Range Max 4m and Min 0.02m 
// Working Freq 40Hz
// Working Current 15mA
// Trigger Signal at least 40 uS


#define micro	1U
#define mili 	1000U
#define sec		1000000U


#define ECHO	(1U<<0);	//PQ0
#define TRIG	(1U<<0);	//PB0

const double _25MHz_1clk = 40e-9; //Value of 1 clk cycle in nanosecond
const unsigned int MULTIPLIER = 5882; 


void delay_Microsecond(unsigned int time);
void Timer4_init(void);


int main(){
	

	delay_Microsecond(10*micro);
	
	return 0;
}


void delay_Microsecond(unsigned int time){
	
	int i;
	SYSCTL->RCGCTIMER |= 0x01; //Enable and provide a clock to 16/32-bit general-purpose timer module 0 in Run mode.
	for(int i = 0; i<1; i++){}
	TIMER0->CTL  = (0U<<0); // TAEN 0. bit and TBEN 8. bit
	TIMER0->TAMR = 0x2; //for Periodic mode.
	TIMER0->TAILR = 25-1; //Load the start value into the GPTM Timer n Interval Load Register (GPTMTnILR).

	TIMER0->ICR = 0x01;// TBTOCINT 8. bit and TATOCINT 0. bit for clearing interupt controller register
		
	TIMER0->CTL  = (1U<<0); // TAEN 0. bit and TBEN 8. bit	
		
	for(unsigned int j =0; j<time; j++){		
		while( (TIMER0->RIS & 0x1) == 0x0 ); // wait for timeout flag to set
		TIMER0->ICR = 0x01;
	}	
	
}


void Timer4_init(void){
	
	SYSCTL->RCGCTIMER |= (1U<<4); //enable clk to Timer Block 4 0b 0001 0000
	SYSCTL->RCGCGPIO |= (1U<<1);	//Enable clk to PORTB
	
	GPIOB_AHB->DIR &= ~ECHO;	//make PB0 an input pin
	GPIOB_AHB->DEN |= ECHO;		//make PB0 a digital pin
	GPIOB_AHB->AFSEL |= ECHO;	//enable alternate function on PB0
	
	GPIOB_AHB->PCTL &= ~(0x0000000F); //configure PB0 as T4CCP0 pin
	GPIOB_AHB->PCTL &= ~(0x00000003);	//Third Functionality is Timerblock
	
	
		//Input Edge Time Mode
	
	// TAEN 0. bit and TBEN 8. bit
	TIMER4->CTL  = (0U<<0); //disable TIMER4A
	
	//For a 16/32-bit timer, this value selects the 16-bit timer configuration.
	TIMER4->CFG = 0x4; 
	
	//Write TACDIR to 0x1 for timer starts from value of 0x00 (4th bit)
	//Write TACMR to 0x1 for Edge Time mode (2nd bit)
	//Write a value of 0x03 for Capture mode (0th and 1st bits)
	TIMER4->TAMR = 0x17; //up-count, edge-time, capture mode
	
//	TIMER4->TAMATCHR = 0xFFFF;	//set the count limit, compared to TAR to determine match evetn
//	TIMER4->TAPMR = 0xFF; // used with TAMATCHR to expand to 0xFFFF FF with prescaler
	
	// GPTM Event Mode 3:2
	// The TAEVENT values are defined as follows
	// 0x0 Positive edge
	// 0x1 Negative edge
	// 0x2 Reserved
	// 0x3 Both Edges
	
	TIMER4->CTL |= (3U<<2); //Both Edges

	TIMER0->CTL  = (1U<<0); // TAEN 0. bit and TBEN 8. bit	 
	
	
}
	

unsigned int measureDistance(void){
	
	GPIOB_AHB->DATA &= ~TRIG;
	delay_Microsecond(12*micro);
	GPIOB_AHB->DATA |= TRIG;
	delay_Microsecond(12*micro);
	GPIOB_AHB->DATA &= ~TRIG;
	
	//Capture first Edge i.e. rising Edge
	TIMER4->ICR = (1U<<2); //Capture Event RAW IS CAERIS (2.bit)
	
	while((TIMER4->RIS & 4)==0){}; //Wait till captured
	
	highEdge = TIMER4->TAR;	
	TIMER4->ICR = (1U<<2); //Clear Timer Capture Flag
	
	while((TIMER4->RIS & 4)==0){}; //Wait till captured
	lowEdge = TIMER4->TAR;
		
	ddistance = lowEdge - highEdge;
	ddistance = _25MHz_1clk *(double)MULTIPLIER *(double)ddistance;
		
	return ddistance;
		
	
}














