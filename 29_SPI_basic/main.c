#include "TM4C129.h"                    // Device header

void delay(unsigned int time);
void SSI_init(void);
void SSI2Write(unsigned char data);

int main(){
	
	
	SSI_init();
	unsigned char i;
	while(1){
		
		for(i = 'A'; i<='z'; i++){
			GPIOD_AHB->DATA &=~(1U<<2);
			SSI2Write(i);
			GPIOD_AHB->DATA |= (1U<<2);
			delay(500*1e3);
		}
	
	}
	
	
}

void delay(unsigned int time){
	
	unsigned int j=0;
	SYSCTL->RCGCTIMER |= 0x04; //TIMER2A
	while( (SYSCTL->PRTIMER &(1U<<2)) != (1U<<2)) {}; //Allow time to finish activating 
	
	TIMER2->CTL &= ~(1U<<0);
	TIMER2->CFG = 0x04;

	TIMER2->TAMR = 0x02;
	TIMER2->TAILR = 16-1;

	TIMER2->ICR =0x01;
	TIMER2->CTL |= (1U<<0);
		
	for (j=0; j<time; j++){
		while((TIMER2->RIS &0x1) == 0x00){	};
		TIMER2->ICR = 0x01;
	}
	

}

void SSI_init(void){
	//Page 1241
	//Step 1
	SYSCTL->RCGCSSI |= (1U<<2); //SSI2 will be used
	while( (SYSCTL->PRSSI & (1U<<2)) != (1U<<2) ) {}; //whether the SSI modules are ready to be accessed by software
	//Step 2
	SYSCTL->RCGCGPIO |= (1U<<3); //PORTD opened for SSI signals
	while( (SYSCTL->PRGPIO &(1U<<3)) != (1U<<3) ) {}; //Allow time to finish activating GPIOD
	
	
	GPIOD_AHB->DIR |= (1U<<2); // PD2 SSI2Fss OUTPUT	
		
	//Step 3
	GPIOD_AHB->AFSEL |= ((1U<<1) | (1U<<2) | (1U<<3)); //Alternate Function
	//Step 4
	GPIOD_AHB->PCTL &= ~((1U<<1) | (1U<<2) | (1U<<3)); //configure PD3, PD2 and PD1
	GPIOD_AHB->PCTL |= ((0xF<<4) | (0xF<<8) | (0xF<<12));	//15th Functionality is SSI2 4 bit for ecery pin
	//Step 5
	GPIOD_AHB->ODR &= ~((1U<<1) | (1U<<2) | (1U<<3));
		
	GPIOD_AHB->AMSEL &= ~( (1U<<1) | (1U<<2) | (1U<<3) ); //Disable analog for PD3, PD2 and PD1
	GPIOD_AHB->DEN |= ((1U<<1) | (1U<<2) | (1U<<3)); // PD3 SS2CLK, PD2 SSI2Fss and PD1 SSI2XDAT0(SSI2Tx) Digitally enable


	GPIOD_AHB->DATA |= (1U<<2);


	
	//QSSI configuration		
	/*
	*		SPI Master, CPOL = 0, CPHA = 0, Clk = 2 MHz, 8 bit data
	*/
	//-----------------------------------------------------------	
	//Step 1/2
	/*ensure that the SSE bit in the SSICR1 register is clear before making any configuration changes*/
	/*
		Bit		Description
		11		Stop Frame : 0 No change is transmission status.
					This bit is applicable when MODE is set to Advanced, Bi- or Quad- SSI.
					This bit is inserted into bit 12 of the TXFIFO data entry by the QSSI
					module.
		
		10  	FSS Hold Frame: 0
					Pulse SSInFss at every byte (the DSS bit in the SSICR0 register
					must be set to 0x7 (data size 8 bits) in this configuration)
		
		9			High Speed Clock Enable: 0 Use Input Clock 
					High speed clock enable is available only when operating as a master.
		
		8			SSI Diretion of Operation : 0 TX (Transmit Mode) write direction
		
		7:6		SSI Mode : 0x0 Legacy SSI mode
		
		2			SSI Master/Slave Select :  The QSSI is configured as a master.
					This bit selects Master or Slave mode and can be modified only when
					the QSSI is disabled (SSE=0).
	
		1			SSI Synchronous Serial Port Enable : 0 QSSI operation is disabled.
		
		0			SSI Loopback Mode : 0 Normal serial port operation enabled.
		
		
		*/		
	SSI2->CR1 &= ~0xFF; // disable SS2
	
	//Step 3
	/* clock source */
	/*
		Bit		Description
		3:0		SSI Baud Clock Source: 0x0 System clock (based on clock source and divisor factor
					programmed in RSCLKCFG register in the System Control Module)
					Specifies the source that generates for the QSSI baud clock
	*/
	SSI2->CC &= ~0xFF;
	
	//Step 4
	/*Configure the clock prescale divisor*/
	/*
		Bit   Description
		7:0		CPSDVSR: SSI Clk Prescale Divisor
					This value must be an even number from 2 to 254, depending on the
					frequency of SSInClk. The LSB always returns 0 on reads	
	*/
	/* SSInClk = SysClk / (CPSDVSR * (1 + SCR)) */
	SSI2->CPSR |= 0x08; //prescaler divided by 8 2MHz
	
	//Step 5
	/* Configure serial Clk Rate, Clk Phase and Polarity, Protocol Mode and Data Size */
	/*
		Bit   Description
		15:8	Serial Clk Rate : 
					This bit field is used to generate the transmit and receive bit rate of the
					SSI. The bit rate is:	BR=SysClk/(CPSDVSR * (1 + SCR)) where CPSDVSR is an 
					even value from 2-254 programmed in the SSICPSR register, and SCR is a value from 0-255.
		7     Serial Clk Phase : 0 Data is captured on the first clock edge transition.
		6			Serial Clk Polarity :  A steady state Low value is placed on the SSInClk pin.
		5:4		SSI Frame Format Select: 0x0 Freescale SPI Frame Format
		3:0   SSI Data Size : 0x7 8-bit data

	*/
	SSI2->CR0 |= 0x7; //2 MHz SSI clk, SPImode, 8 bit data

	
	//Step 7
	SSI2->CR1 |= (1U<<1) ; //Enable SSI2
		
		
}


void SSI2Write(unsigned char data){

	/*Status Register*/
	/*
		Bit   Description
		4			SSI Busy Bit
					0 The QSSI is idle.
					1 The QSSI is currently transmitting and/or receiving a frame, or the transmit FIFO is not empty.
		
		3			SSI Receive FIFO Full
					0 The receive FIFO is not full.
					1 The receive FIFO is full.
	
		2			SSI Receive FIFO Not Empty
					0 The receive FIFO is empty.
					1 The receive FIFO is not empty
	
		1			SSI Transmit FIFO Not Full
					0 The transmit FIFO is full.
					1 The transmit FIFO is not full
	
		0			SSI Transmit FIFO Empty
					0 The transmit FIFO is not empty.
					1 The transmit FIFO is empty.
	
	*/
	while( (SSI2->SR & (1U<<1)) == 0) {}; //wait for not full
		
	/*
		SSI Receive/Transmit Data
		A read operation reads the receive FIFO. A write operation writes the transmit FIFO.
		Software must right-justify data when the QSSI is programmed for a data size that is 
		less than 16 bits. Unused bits at the top are ignored by the transmit logic. 
		The receive logic automatically right-justifies the data.
				
		*/
		
	SSI2->DR |= (0x00FF & data); //8 bits wide
	
	while( (SSI2->SR & (1U<<4)) ) {};//wait for empty
			
}







