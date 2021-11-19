#include "TM4C129.h"                    // Device header

// Sample sequences because of FIFO size
// SS0 -> 8 bit samples FIFO depth -> 8
// SS1 -> 4 bit samples FIFO depth -> 4
// SS2 -> 4 bit samples FIFO depth -> 4
// SS3 -> 1 bit samples FIFO depth -> 1

//we going to use ss3 because of a single channel

//ADCACTSS
//ADCEMUX
//ADCPSSI
//ADCRIS

//Pin
//Name		Number Assignment Buffer_type
//AIN0 		12 		PE3 I Analog Analog-to-digital converter input 0.
//AIN1 		13 		PE2 I Analog Analog-to-digital converter input 1.
//AIN2 		14 		PE1 I Analog Analog-to-digital converter input 2.
//AIN3 		15 		PE0 I Analog Analog-to-digital converter input 3.
//AIN4 		128 	PD7 I Analog Analog-to-digital converter input 4.
//AIN5 		127 	PD6 I Analog Analog-to-digital converter input 5.
//AIN6 		126 	PD5 I Analog Analog-to-digital converter input 6.
//AIN7 		125 	PD4 I Analog Analog-to-digital converter input 7
//AIN8 		124 	PE5 I Analog Analog-to-digital converter input 8.
//AIN9 		123 	PE4 I Analog Analog-to-digital converter input 9.
//AIN10 	121 	PB4 I Analog Analog-to-digital converter input 10.
//AIN11 	120 	PB5 I Analog Analog-to-digital converter input 11.
//AIN12 	4 		PD3 I Analog Analog-to-digital converter input 12.
//AIN13 	3 		PD2 I Analog Analog-to-digital converter input 13.
//AIN14 	2 		PD1 I Analog Analog-to-digital converter input 14.
//AIN15 	1 		PD0 I Analog Analog-to-digital converter input 15.
//AIN16 	18 		PK0 I Analog Analog-to-digital converter input 16.
//AIN17 	19 		PK1 I Analog Analog-to-digital converter input 17.
//AIN18 	20 		PK2 I Analog Analog-to-digital converter input 18.
//AIN19 	21 		PK3 I Analog Analog-to-digital converter input 19.



// PE3 -> POT2
// 3v3 -> POT1
// GND -> POT3

int result =0;

int main(){
	
	SYSCTL->RCGCGPIO	|= (1U<<4);		//Enable clk for PORT E
	SYSCTL->RCGCADC	 	|= (1U<<0);		//Enable clk for ADC module 0
	
	//PE3 init
	GPIOE_AHB->AFSEL  |= (1U<<3);		//Enable alternate function AIN0 at PE3
	GPIOE_AHB->DEN		&=~(1U<<3);		//Disable Digital Function
	GPIOE_AHB->AMSEL	|= (1U<<3);		//Enable Analog Function
	
	
//Sample Sequencer Configuration
//Configuration of the sample sequencers is slightly more complex than the module initialization
//because each sample sequencer is completely programmable.
//	
//The configuration for each sample sequencer should be as follows:
//	
//1. Ensure that the sample sequencer is disabled by clearing the corresponding ASENn bit in the
//ADCACTSS register. Programming of the sample sequencers is allowed without having them
//enabled. Disabling the sequencer during programming prevents erroneous execution if a trigger
//event were to occur during the configuration process.
//	
//2. Configure the trigger event for the sample sequencer in the ADCEMUX register.
//
//3. When using a PWM generator as the trigger source, use the ADC Trigger Source Select
//(ADCTSSEL) register to specify in which PWM module the generator is located. The default
//register reset selects PWM module 0 for all generators.
//
//4. For each sample in the sample sequence, configure the corresponding input source in the
//ADCSSMUXn and ADCSSEMUXn registers.
//
//5. For each sample in the sample sequence, configure the sample control bits in the corresponding
//nibble in the ADCSSCTLn register. When programming the last nibble, ensure that the END bit
//is set. Failure to set the END bit causes unpredictable behavior.
//
//6. If interrupts are to be used, set the corresponding MASK bit in the ADCIM register.
//
//7. Enable the sample sequencer logic by setting the corresponding ASENn bit in the ADCACTSS
//register.
	
	
	//ADC Init
	ADC0->ACTSS 			&=~(1U<<3);		//Disable Sample Sequencer 3 (SS3)	(Step 1)		
	ADC0->EMUX				&=~(0xF000);	//Selects the event (trigger) that initiates (Step 2)
																	//sampling for each sample sequencer
																	//SS3 Trigger Select Third Nibble 
																	//The trigger is initiated by setting the SSn 
																	//bit in the ADCPSSI register
	ADC0->SSMUX3			=		0;				//Get inout from channel 0 (Step 4)
																	//First Sample Input Select
	ADC0->SSCTL3			|= (1U<<1)|		//(Step 5)
											 (1U<<2);		//Take one sample at a time, set flat at 1st sample
																	// 3 	 2 	 1 		0
																	//TS0 IE0 END0 D0  
	ADC0->ACTSS 			|= (1U<<3);		//Enable Sample Sequencer 3 (SS3)	(Step 7)	
	
	
	
	while(1){
		
		ADC0->PSSI |= (1U<<3);				//initiate sampling in the sample sequencers
		
		while( ((ADC0->RIS)&(1U<<3)) == 0 ) {}	//SS3 Raw Interrupt Status 
																						//Wait for conversion complete
		result = ADC0->SSFIFO3; // get the result 
		ADC0->ISC = (1U<<3); 		// cleared by writing a 1 to the IN3 bit in the ADCISC register.
	}
	
}




