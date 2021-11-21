#include "TM4C129.h"                    // Device header

void Timer0A_init(void);
void ADC0_init(void);
unsigned int ADC0_InSeq3(void);

unsigned int result;
int main(){

	__disable_irq();
	ADC0_init();
	Timer0A_init();
	__enable_irq();
	
	while(1){
		ADC0_InSeq3();
	}
	
}


void ADC0_init(void){
	SYSCTL->RCGCADC |= (1U<<0);//Provided a clock and accesses to module registers are allowed
	while((SYSCTL->PRADC & (1U<<0)) != (1U<<0)) {}; //whether the ADC modules are ready to be accessed by software
	
	ADC0->PC &= ~(0xF);	//Provides information regarding the configuration of the peripheral.
	ADC0->PC |= (0x1);	//Eighth conversion rate (112Tadc) 125K sample for period
	
	ADC0->SSPRI |= 0x0123;	//Sets the priority for each of the sample sequencers (Sequencer 3 is highest prio )
	ADC0->ACTSS &= ~(1U<<3); //Controls the activation of the sample sequencers (Sample Sequencer 3 is disabled)
	ADC0->EMUX &= ~0xF000; //Selects the trigger that initiates sampling for each sample sequencer (The trigger is initiated by setting the SS3 bit in the ADCPSSI register.)
	ADC0->EMUX |= (0x5000); //Configure for timer trigger event
	ADC0->SSCTL3 |= (1U<<3) | (1U<<2) | (1U<<1); //Contains the configuration information for a sample (Sample Interrupt Enable and End of Sequence(MUST))
	ADC0->IM |= (1U<<3); //Controls whether the sample sequencer and digital comparator raw interrupt signals are sent to the interrupt controller.(SS3 Interrupt Mask)
	ADC0->ACTSS |= ~(1U<<3);	//(Sample Sequencer 3 is enabled)

	NVIC->IP[49] = (0x03);	// set interrupt to priority 3
	NVIC->ISER[1] = (1U<<17);	//Enable IRQ49 32+17

}



unsigned int ADC0_InSeq3(void){
	ADC0->PSSI |= (1U<<3); //Provides a mechanism for application software to initiate sampling in the sample sequencers (SS3 Initiate)
	while((ADC0->RIS & (1U<<3)) == 0){}; //shows the status of the raw interrupt signal of each sample sequencer(SS3 Raw Interrupt Status)
	result = ADC0->SSFIFO3 & 0xFFF; //Contains the conversion results for samples collected with the sample sequencer
	ADC0->ISC = (1U<<3);	//Provides to clear SS IRQ conditions and shows the status of IRQs generated by the SS (SS3 Interrupt Status and Clear)
	return result;
}

void Timer0A_init(void){
	SYSCTL->RCGCTIMER |= (1U<<0); //activate Timer0
	while( (SYSCTL->PRTIMER &(1U<<0)) != (1U<<0)) {}; //Allow time to finish activating 
		
	TIMER0->CTL &= ~(1U<<0);	//disable timer0A during setup
	TIMER0->CTL |= (1U<<5); //enable timer0A tirgger to ADC
	TIMER0->CFG = 0x04; //Configure for 16 bit timer mode
	//timer0A initialization
	TIMER0->TAMR |= 0x02; //periodc ,count down
	TIMER0->TAPR |= 0xFF;	//prescaler 256
	TIMER0->TAILR |= 0xFFFF; //65k start value for trigger
	TIMER0->IMR &= (0x01); //disable timout irq
	TIMER0->CTL |= (1U<<0); //enable timer0A, 16b, periodic, no irq
}



