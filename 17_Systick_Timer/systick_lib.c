#include "TM4C129.h"                    // Device header
#include "systick_lib.h"

void SysTick_Init(unsigned int us){
	
	unsigned int load_val = us*25;
	
	SysTick->LOAD = load_val - 1 ; //same as mcu clk freq so every 1 sec

	
	//Clock Source, Interrupt Enable, Enable
	// 1						0 								1 = 0x5
	
	//clk_src  0--> Precision internal oscillator (PIOSC) divided by 4
	//				 1--> System clock
	//inten		 0--> Interrupt generation is disabled. Software can use the COUNT bit(16th bit) to determine if the counter has ever reached 0.
	//				 1--> An interrupt is generated to the NVIC when SysTick counts to 0.
	//enable   0--> The counter is disabled.
	//				 1--> Enables SysTick to operate in a multi-shot way. That is, the
	//							counter loads the RELOAD value and begins counting down.
	//							On reaching 0, the COUNT bit is set and an interrupt is
	//							generated if enabled by INTEN. The counter then loads the
	//							RELOAD value again and begins counting.
	SysTick->CTRL = 0x5; 
	
}

bool SysTick_Timeout(void){
	
	// COUNT (16th bit)  0--> The SysTick timer has not counted to 0 since the last time this bit was read.
	// 									 1--> The SysTick timer has counted to 0 since the last time this bit was read.
	return (SysTick->CTRL & 0x10000);
	
}



