#include "TM4C129.h"                    // Device header

#define testLED (1U<<4) //PF4

//1. Trig den en az 10 us lik darbe gönder

#define ECHO	(1U<<0)		//PB0
#define TRIG	(1U<<0)		//PQ0
#define LED4	(1U<<0)		//PF0

#define micro	1U
#define mili 	1000U
#define sec		1000000U


void Ports_Init(void){
	//TRIG PORT (PQ0) INIT
	SYSCTL->RCGCGPIO |= (1U<<14); //PQ for Trigger clk enabled
	while( (SYSCTL->PRGPIO &(1U<<14)) != (1U<<14)) {}; //Allow time to finish activating GPIO
	GPIOQ->DIR |= TRIG; //output for Trigger pin
	GPIOQ->DEN |= TRIG;
	
	SYSCTL->RCGCGPIO |= (1U<<1);	//Enable clk to PORTB	
	while( (SYSCTL->PRGPIO &(1U<<1)) != (1U<<1)) {}; //Allow time to finish activating GPIO

	GPIOB_AHB->DIR &= ~ECHO;	//make PB0 an input pin
	GPIOB_AHB->DEN |= ECHO;		//make PB0 a digital pin
	
	GPIOB_AHB->AFSEL |= ECHO;	//enable alternate function on PB0
	GPIOB_AHB->PCTL &= ~(0x0000000F); //configure PB0 as T4CCP0 pin
	GPIOB_AHB->PCTL |= (0x00000003);	//Third Functionality is Timerblock 
	//(Notice the paranthesis in datasheet for PB0) 

	//LED4 (PF0) INIT
	SYSCTL->RCGCGPIO |= (1U<<5);	//PF for LED4
	while( (SYSCTL->PRGPIO &(1U<<5)) != (1U<<5)) {}; //Allow time to finish activating GPIO
	GPIOF_AHB->DIR |= (LED4 | testLED); // ouput for PF0 LED4
	GPIOF_AHB->DEN |= (LED4 | testLED);
	
}


void delay_Microsecond(unsigned int time){
	unsigned int j =0;
	SYSCTL->RCGCTIMER |= 0x01; //Enable and provide a clock to 16/32-bit general-purpose timer module 0 in Run mode.
	while( (SYSCTL->PRTIMER &(1U<<0)) != (1U<<0)) {}; //Allow time to finish activating 

	TIMER0->CTL  &= ~(1U<<0); // TAEN 0. bit and TBEN 8. bit

	//For a 16/32-bit timer, this value selects the 16-bit timer configuration.
	TIMER0->CFG = 0x04; 
		
	TIMER0->TAMR = 0x2; //for Periodic mode.
	TIMER0->TAILR = 16-1; //Load the start value into the GPTM Timer n Interval Load Register (GPTMTnILR).

	TIMER0->ICR = 0x01;// TBTOCINT 8. bit and TATOCINT 0. bit for clearing interupt controller register
		
	TIMER0->CTL  |= (1U<<0); // TAEN 0. bit and TBEN 8. bit	
	
	
	for(j =0; j<time; j++){		
		while( (TIMER0->RIS & 0x1) == 0x0 ){ };// wait for timeout flag to set
		TIMER0->ICR = 0x01;
	}	
	
}


unsigned int highEdge, lowEdge;

void  trigSignal(void){
	
	// We will start to send sonic Burst so trigger the module
	GPIOQ->DATA &= ~TRIG;
	delay_Microsecond(10*micro);
	GPIOQ->DATA |= TRIG;
	delay_Microsecond(10*micro);
	GPIOQ->DATA &= ~TRIG;
	

}




//2. 40Khz  sinyal sensör tarafindan üretilir
//3. Echo sinyali High olur (38 ms Timeout degeridir)
//4. Yanki geldiginde Echo low olur 


void Timer4_conf(void){
	
	//Input Edge Time Mode
	
	//TIMER4 PORT (PB0) INIT
	SYSCTL->RCGCTIMER |= (1U<<4); //enable clk to Timer Block 4 0b 0001 0000
	while( (SYSCTL->PRTIMER &(1U<<4)) != (1U<<4)) {}; //Allow time to finish activating 
		
	// TAEN 0. bit and TBEN 8. bit
	TIMER4->CTL  &= ~(1U<<0); //disable TIMER4A
	
	//For a 16/32-bit timer, this value selects the 16-bit timer configuration.
	TIMER4->CFG = 0x04; 
	
	//Write TACDIR to 0x1 for timer starts from value of 0x00 (4th bit)
	//Write TACMR to 0x1 for Edge Time mode (2nd bit)
	//Write a value of 0x03 for Capture mode (0th and 1st bits)
	TIMER4->TAMR = 0x07; //down-count, edge-time, capture mode

	// GPTM Event Mode 3:2
	// The TAEVENT values are defined as follows
	// 0x0 Positive edge
	// 0x1 Negative edge
	// 0x2 Reserved
	// 0x3 Both Edges
	TIMER4->CTL |= 0x0C; //Both Edges

	TIMER4->IMR |= (1U<<2); // Capture interrupt enabled (CAEIM)	


	TIMER4->CTL  |= (1U<<0); // TAEN 0. bit and TBEN 8. bit	 
	
}

unsigned int Echo_capture(void){
	
//	//Capture first Edge i.e. rising Edge
	TIMER4->ICR |= (1U<<2); //Timer A Capture Mode Event Interrupt Clear CAECINT (2.bit)	

	GPIOF_AHB->DATA |= testLED;
	
	
	while( (TIMER4->RIS & (1U<<2)) == (0U<<2) ){//rising edge//Wait till captured
		
	}	

	
	if( ( GPIOB_AHB->DATA & (1U<<0)) == (1U<<0) )
		highEdge = TIMER4->TAR; //highEdge = the time at which last edge event took place
	else
		highEdge =0;
	
	TIMER4->ICR |= (1U<<2); //Clear Timer Capture Flag
		
	while( (TIMER4->RIS & (1U<<2)) == (0U<<2) ){//falling edge//Wait till captured
		
	}	
		
	if( (GPIOB_AHB->DATA & (1U<<0)) == (0U<<0) )
		lowEdge = TIMER4->TAR; //highEdge = the time at which last edge event took place
	else
		lowEdge =0;
	
	
	GPIOF_AHB->DATA &= ~testLED;

	TIMER4->ICR |= (1U<<2); //Clear Timer Capture Flag


//	if(lowEdge <= highEdge){
//		return (65535 - highEdge)+lowEdge;
//	}else{
//		return (lowEdge - highEdge) & 0x0000FFFF;
//	}		
	return (highEdge - lowEdge) & 0x0000FFFF;

	//return time difference
}





//5. Uzaklik = Hiz x Zaman => 0.034cm/s * (örnegin 500us) / 2 ==> çünkü gitti geldi
//const unsigned int _16MHz_1clk = 625; //1 clk cycle 62.5 nanosecond

unsigned int distance(unsigned int number_of_clk_diff){
	
//	long long unsigned int time_period = number_of_clk_diff * _16MHz_1clk; //in terms of second
	
	 //(m/s) *   (cm/m)  
	//340   *  (1/10e-2)  / 2; => cm/sn
//	unsigned int  MULTIPLIER = 17000; 

//	unsigned int MULTIPLIER = 10625; // x10^-7
	unsigned int DIVIDER = 941; // x10^-7
	
//	unsigned int result = (((long long unsigned int)MULTIPLIER*(long long unsigned int)number_of_clk_diff)/1e7);
	unsigned int result = number_of_clk_diff / DIVIDER;
	
	return result;
}


unsigned int measureDistance(void){
	
	//1. Trig den en az 10 us lik darbe gönder

	trigSignal();
	
	//2. 40Khz  sinyal sensör tarafindan üretilir
	//3. Echo sinyali High olur (38 ms Timeout degeridir)
	//4. Yanki geldiginde Echo low olur 

	unsigned int number_of_clk_diff = Echo_capture();

	//5. Uzaklik = Hiz x Zaman => 0.034cm/s * (örnegin 500us) / 2 ==> çünkü gitti geldi
	unsigned int dist = distance(number_of_clk_diff);
	
	return dist;
	
}


int main(){
	

//	SYSCTL->RCGCGPIO |= (1U<<5);	//PF
//	while( (SYSCTL->PRGPIO &(1U<<5)) != (1U<<5)) {}; //Allow time to finish activating GPIO	
//	GPIOF_AHB->DIR |= testLED; // ouput 
//	GPIOF_AHB->DEN |= testLED;
	
	int distance_in_cm = 0;
	__disable_irq();

	Timer4_conf(); // PB0 for echo signal	
	Ports_Init();
	
	__enable_irq();
	
	while(1){

		distance_in_cm = measureDistance();
		
			if(distance_in_cm <= 15){
				GPIOF_AHB->DATA |= LED4;
			}else{
				GPIOF_AHB->DATA &= ~LED4;
			}
		delay_Microsecond(1*sec);
	}
	
	
	return 0;
}

