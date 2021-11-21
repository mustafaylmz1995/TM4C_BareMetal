#include "TM4C129.h"                    // Device header


#define LED1 (1U<<1)	//PN1
#define LED2 (1U<<0)	//PN0

#define MS2S 	1000U
#define US2S 	1000000U
#define US2MS	1000U


void GPTM_Timer0_oneshot(unsigned int us);


int main(){
	
	SYSCTL->RCGCGPIO |= (1U<<12); //N_Portu=>12.bit
	//LEDs->output
	GPION->DIR |= (LED1 | LED2);
	//Digital Enable
	GPION->DEN |= (LED1 | LED2);
	//Initial Data
	GPION->DATA |= (LED1 | LED2);
	
	
	while(1){
		
		GPION->DATA ^= (LED2);
		for(int i = 0; i<500; i++){
			GPTM_Timer0_oneshot(2*US2MS);
		}
		GPION->DATA ^= (LED1);
		for(int i = 0; i<500; i++){
			GPTM_Timer0_oneshot(2*US2MS);
		}
	}
	
}


//max 171 sec 798 ms 691 us can be sent
//max 2621 usec for 16 bit timer
void GPTM_Timer0_oneshot(unsigned int us){ // Send us type, for instance 1 sec is equal to 1*10^9 
	
	
	//When enabled, a module is provided a clock and accesses to module registers
	//are allowed. When disabled, the clock is disabled to save power and accesses to module registers
	//generate a bus fault.
	
	//Timers  7  6  5  4  3  2  1  0
	//Regs		R7 R6 R5 R4 R3 R2 R1 R0

	SYSCTL->RCGCTIMER |= 0x01; //Enable and provide a clock to 16/32-bit general-purpose timer module 0 in Run mode.
	while( (SYSCTL->PRTIMER &(1U<<0)) != (1U<<0)) {}; //Allow time to finish activating 
	
	//1. Ensure the timer is disabled (the TnEN bit in the GPTMCTL register is cleared) before making
	//any changes.
	// TAEN 0. bit and TBEN 8. bit
	TIMER0->CTL  = (0U<<0);

	//2. Write the GPTM Configuration Register (GPTMCFG) with a value of 0x0000.0000.
	//GPTMCFG has 3 bits -> R2 R1 R0 
	// 0x0	For a 16/32-bit timer, this value selects the 32-bit timer configuration.
	// 0x1	For a 16/32-bit timer, this value selects the 32-bit real-time clock (RTC) counter configuration.
	// 0x2-0x3	Reserved
	// 0x4	For a 16/32-bit timer, this value selects the 16-bit timer configuration. 
	// The function is controlled by bits 1:0 of GPTMTAMR and GPTMTBMR.
	// 0x5-0x7	Reserved	
	TIMER0->CFG = 0x4; //For a 16/32-bit timer, this value selects the 16-bit timer configuration.
	
	//3. Configure the TnMR field in the GPTM Timer n Mode Register (GPTMTnMR):
	//a. Write a value of 0x1 for One-Shot mode.
	//b. Write a value of 0x2 for Periodic mode.
	TIMER0->TAMR = 0x1; //for One-Shot mode.

	//4. Optionally configure the TnSNAPS, TnWOT, TnMTE, and TnCDIR bits in the GPTMTnMR register
	//to select whether to capture the value of the free-running timer at time-out, use an external
	//trigger to start counting, configure an additional trigger or interrupt, and count up or down. In
	//addition, if using CCP pins, the TCACT field can be programmed to configure the compare action.
	//NOTUSED

	//5. Load the start value into the GPTM Timer n Interval Load Register (GPTMTnILR).
	TIMER0->TAILR = us*16 -1;
	
	//6. If interrupts are required, set the appropriate bits in the GPTM Interrupt Mask Register
	//(GPTMIMR).
	//NOT USED
	
	// An additional step for clearing interupt controller register
	// TBTOCINT 8. bit and TATOCINT 0. bit
	// GPTM Timer A/ Timer B Time-Out Raw Interrupt
	// Writing a 1 to this bit clears the TATORIS bit in the GPTMRIS register
	// and the TATOMIS bit in the GPTMMIS register.
	TIMER0->ICR = 0x01;

	//7. Set the TnEN bit in the GPTMCTL register to enable the timer and start counting.
	// TAEN 0. bit and TBEN 8. bit
	TIMER0->CTL |= (1U<<0);
	
	//8. Poll the GPTMRIS register or wait for the interrupt to be generated (if enabled). In both cases,
	//the status flags are cleared by writing a 1 to the appropriate bit of the GPTM Interrupt Clear
	//Register (GPTMICR)
	// Reg0    Reg4    Reg8    Reg11
	// TATORIS TAMRIS  TBTORIS TBMRIS
	// TATORIS / TBTORIS :
	// 0-> Timer A/ Timer B	has not timed out.
	// 1-> Timer A/Timer B has timed out. This interrupt is asserted when a
	// one-shot or periodic mode timer reaches it's count limit (0 or
	// the value loaded into GPTMTAILR, depending on the count
	// direction).
	while( (TIMER0->RIS & 0x1) == 0x0 ); // wait for timeout flag to set

}

