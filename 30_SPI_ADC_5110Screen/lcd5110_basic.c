#include "LCD5110_Basic.h"
#include "TM4C129.h"                    // Device header
#include "delay.h"
#include <string.h>
#include <stdio.h>

//SSI2 will be used
//TM4C1294															5110 LCD
//PD3 SS2CLK -------------------------> Clk		
//PD2 SSI2Fss	------------------------> CE (active low)
//PD1 SSI2XDAT0(SSI2Tx) --------------> Din 
//PD0 SSI2XDAT1 (SSI2Rx) -------------> -
//3v3 --------------------------------> Vcc
//GND --------------------------------> GND
//PE0 --------------------------------> RST (active low)
//PE1 --------------------------------> DC ( Command(0)/Data(1) )


__current_font cfont;
boolean __sleep;
unsigned int __contrast;


void GPIO_init(void){
	SYSCTL->RCGCGPIO |= (1U<<4); //PORTE opened for RST and DC signals
	while( (SYSCTL->PRGPIO &(1U<<4)) != (1U<<4) ) {}; //Allow time to finish activating GPIOE
	GPIOE_AHB->DIR  |= ( (1U<<0) | (1U<<1) );
	GPIOE_AHB->AFSEL &= ~( (1U<<0) | (1U<<1) );
	GPIOE_AHB->ODR &= ~( (1U<<0) | (1U<<1) );
	GPIOE_AHB->DEN |= ( (1U<<0) | (1U<<1) );
	GPIOE_AHB->DATA |= (1U<<0); //reset active low
	GPIOE_AHB->DATA &=~(1U<<1); // Command(0)/Data(1)
}

void SSI_init(void){
	
	SYSCTL->RCGCSSI |= (1U<<2); //SSI2 will be used
	while( (SYSCTL->PRSSI & (1U<<2)) != (1U<<2) ) {}; //whether the SSI modules are ready to be accessed by software
	SYSCTL->RCGCGPIO |= (1U<<3); //PORTD opened for SSI signals
	while( (SYSCTL->PRGPIO &(1U<<3)) != (1U<<3) ) {}; //Allow time to finish activating GPIOD

		
	GPIOD_AHB->DIR |= ( (1U<<1) | (1U<<2) | (1U<<3) );

	GPIOD_AHB->AFSEL |= ((1U<<1) | (1U<<3) | (1U<<2)); //Alternate Function
	GPIOD_AHB->PCTL &= ~((1U<<1) | (1U<<3) | (1U<<2)); //configure PD3 and PD1 and PD2
	GPIOD_AHB->PCTL |= ((0xF<<4 ) | (0xF<<8) | (0xF<<12));	//15th Functionality is SSI2 
	
	GPIOD_AHB->ODR &= ~( (1U<<1) | (1U<<2) | (1U<<3) );
	
	GPIOD_AHB->AMSEL &= ~((1U<<1) | (1U<<3) | (1U<<2)); //Disable analog for PD3 SS2CLK and PD1 SSI2XDAT0(SSI2Tx) and PD2 SSI2Fss
	GPIOD_AHB->DEN |= ((1U<<1) | (1U<<3) | (1U<<2)); // PD3 SS2CLK and PD1 SSI2XDAT0(SSI2Tx) and PD2 SSI2Fss Digitally enable


		
	/*
	*		SPI Master, CPOL = 0, CPHA = 0, Clk = 2 MHz, 8 bit data
	*/
	//-----------------------------------------------------------	
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
	
	
	/* clock source */
	/*
		Bit		Description
		3:0		SSI Baud Clock Source: 0x0 System clock (based on clock source and divisor factor
					programmed in RSCLKCFG register in the System Control Module)
					Specifies the source that generates for the QSSI baud clock
	*/
	SSI2->CC &= ~0xFF;
	
	
	/*Configure the clock prescale divisor*/
	/*
		Bit   Description
		7:0		CPSDVSR: SSI Clk Prescale Divisor
					This value must be an even number from 2 to 254, depending on the
					frequency of SSInClk. The LSB always returns 0 on reads	
	*/
	/* SSInClk = SysClk / (CPSDVSR * (1 + SCR)) */
	SSI2->CPSR |= 0x08; //prescaler divided by 2 2MHz
	
	
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
	SSI2->CR0 |= 0x7; //8 MHz SSI clk, SPImode, 8 bit data

	SSI2->CR1 |= (1U<<1); //Enable SSI2
		
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
	while( !((SSI2->SR & (1U<<1)) == (1U<<1))) {}; //wait for not full
		
	/*
		SSI Receive/Transmit Data
		A read operation reads the receive FIFO. A write operation writes the transmit FIFO.
		Software must right-justify data when the QSSI is programmed for a data size that is 
		less than 16 bits. Unused bits at the top are ignored by the transmit logic. 
		The receive logic automatically right-justifies the data.
				
		*/
		
	SSI2->DR |= (0x00FF & data); //8 bits wide

	while( ((SSI2->SR) & (1U<<4)) ) {}; //wait for not busy

}

void MCU_start(void){
	GPIO_init();
	SSI_init();
}

void resetLCD(void){
	GPIOE_AHB->DATA &= ~(1U<<0); //reset active low
	delay(150*MS);
	GPIOE_AHB->DATA |= (1U<<0);
}

void __LCD_WRITE(unsigned char data, unsigned char mode){
	//LCD start
	GPIOD_AHB->DATA &=~(1U<<2); //PD2 SSI2Fss
	if(mode == LCD_COMMAND){
		GPIOE_AHB->DATA &=~(1U<<1); //PE1 DC
	}else{
		GPIOE_AHB->DATA |=(1U<<1); //PE1 DC
	}
	
	SSI2Write(data); //MOSI 
	
	GPIOD_AHB->DATA |= (1U<<2); //PD2 SSI2Fss
		
}

void LCD5510_initLCD(int contrast){
	MCU_start();
	
	if(contrast>0x7F)
		contrast = 0x7F;
	
	if(contrast <0)
		contrast = 0;
	
	// Initialization
	resetLCD(); //1. Reset Res pin active low there should be 100ms wait time
	
	__LCD_WRITE(PCD8544_FUNCTIONSET | PCD8544_EXTENDEDINSTRUCTION, LCD_COMMAND);//Fonksiyonel Ayarlamalar i�in extended instruction acildi
	__LCD_WRITE(PCD8544_SETVOP|contrast, LCD_COMMAND); //Operational Voltage level set for 1:48 Bias setting  Vlcd = 6.06*Vthreshold Vop 00->7F arasinda contrast ayari icin kullanilir
	__LCD_WRITE(PCD8544_SETTEMPCOEF | LCD_TEMP, LCD_COMMAND); //Temperature Coefficient Set 0x02
	__LCD_WRITE(PCD8544_SETBIASSYS|LCD_BIAS, LCD_COMMAND); //Set 1:48 bias recommended mux rate 


	__LCD_WRITE(PCD8544_FUNCTIONSET, LCD_COMMAND); //Func seti temizledik
	__LCD_WRITE(PCD8544_SETYADDR, LCD_COMMAND); //0-5 arasi bankler icin Ram adresi ayarlanir
	__LCD_WRITE(PCD8544_SETXADDR, LCD_COMMAND); // 0-83 arasi satir i�in Ram adresini ayarlar
	


	for(int c = 0; c<504; c++){
		__LCD_WRITE(0x00, LCD_DATA);	//T�m bank ve Satirlar i�in Ramdeki degerler 0 degeri ile dolduruldu
	}		
	
	__LCD_WRITE(PCD8544_DISPLAYCONTROL|PCD8544_DISPLAYNORMAL, LCD_COMMAND); //Displayi Normal moda �evirdik

	
	
	cfont.font = 0; //font degerini 0 atiyoruz
	__sleep = False; //uyku modu acik degil
	__contrast = contrast; //global contrast degeri g�nderilen degere esitleniyor
	
}

unsigned char fontbyte(unsigned int x){
	return cfont.font[x]; //fontun istenen indeksteki degerini geri d�nd�r�yoruz
}



void __print_char(unsigned char c, int x, int row){
	
	if(!__sleep){ //uyku modu acik degilse
		
		if( ((x+cfont.x_size)<=84) && (row+(cfont.y_size>>3)<=6) ){ //char'in yazilacagi bank ve bit degerini aliyoruz
			
			for(int rowcnt=0; rowcnt<(cfont.y_size>>3); rowcnt++){
				
				__LCD_WRITE(PCD8544_SETYADDR | (row+rowcnt), LCD_COMMAND); // y adresini g�nderilen degere ayarladik
				__LCD_WRITE(PCD8544_SETXADDR | x, LCD_COMMAND);	//x adresini 0 dan bas
				
				int font_idx = ( (c-cfont.offset)*(cfont.x_size *(cfont.y_size>>3)) )+4;
				
				for (int cnt=0; cnt<cfont.x_size; cnt++){
					
					if(cfont.inverted == 0){
						__LCD_WRITE(fontbyte(font_idx + cnt+ (rowcnt*cfont.x_size)), LCD_DATA);						
					}else{
						__LCD_WRITE(~(fontbyte(font_idx + cnt + (rowcnt*cfont.x_size))), LCD_DATA);
					}
				
				}
				
			}
			
			__LCD_WRITE(PCD8544_SETYADDR, LCD_COMMAND);	
			__LCD_WRITE(PCD8544_SETXADDR, LCD_COMMAND);
			
		}
			
	}
	
}

void LCD5110_print(char *st, int x, int y){
	
	int st1, row;
	
	if(!__sleep){
		
		st1 = strlen(st); //string uzunlugu alindi
		 
		if( x == RIGHT )
			x = 84-(st1*cfont.x_size);
		
		if(x == CENTER )
			x = (84 - (st1*cfont.x_size))>>1;
		
		if(x<0)
			x = 0;
		
		row = y>>3;

		
		for(int cnt=0; cnt<st1; cnt++)
			__print_char(*st++, x+(cnt*(cfont.x_size)), row);
		
	}
	
}

void LCD5110_printNumI(long num, int x, int y, int length, char filler){
	
	char buf[25];
	char st[27];
	boolean neg=False;
	int c=0, f=0;
	
	if(!__sleep){
		
		if(num == 0){
			
			if(length!=0){
				
				for (c=0; c<(length-1); c++)
					st[c]=filler;
				
				st[c]=48;
				st[c+1]=0;
				
			}else{
				
				st[0] = 48;
				st[1] = 0;
				
			}
			
			
		}else{
			
			if(num<0){
				neg=True;
				num = -num;
				
			}
			
			while(num>0){
				
				buf[c] = 48 + (num%10);
				c++;
				num = (num - (num %10))/10;
				
			}
			
			buf[c] = 0;
			
			if(neg){
				st[0] = 45;
			
			}
			
			if(length > (c+neg)){
				for(int i=0; i<(length-c-neg); i++){
					st[i+neg] = filler;
					f++;
				}
				
			}
			
			for(int i=0; i<c; i++){
				st[i+neg+f] = buf[c-i-1];
			}
			
			st[c+neg+f] = 0;
		}
		
		
		LCD5110_print(st, x, y);
		
	}
	
}

void __convert_float(char *buf, double num, int width, unsigned char prec){
	char format[10];
	sprintf(format, "%%%i.%if", width, prec);
	sprintf(buf, format, num);
}


void LCD5110_printNumF(double num, unsigned char dec, int x, int y, char divider, int length, char filler){
	
	char st[27];
	boolean neg = False;
	
	if(!__sleep){
		
		if(num<0)
			neg = True;
		
		__convert_float(st, num, length, dec);
		
		if(divider != '.'){
			for(int i =0; i<sizeof(st); i++)
				if( st[i] == '.' )
					st[i] = divider;
		}
		
		if(filler != ' '){
			if(neg){
				st[0] = '-';
				for(int i=1; i<sizeof(st); i++)
					if( ( st[i]==' ' ) || ( st[i]=='-' ) )
						st[i]=filler;
				
			}else{
				for(int i=0; i<sizeof(st); i++)
					if( st[i] == ' ')
						st[i] =filler;
			}
			
		}
		
		LCD5110_print(st, x, y);
		
	}
	
	
}

unsigned char bitmapbyte(unsigned int x, bitmapdatatype bitmap){
	return bitmap[x];
}


void LCD5110_drawBitmap(int x, int y, bitmapdatatype bitmap, int sx, int sy){
	
	int starty, rows;
	
	if(!__sleep){
		starty = y>>3;
		
		if(sy%8 == 0)
			rows = sy>>3;
		else
			rows = (sy>>3)+1;
		
		for(int cy=0; cy<rows; cy++){
			__LCD_WRITE(PCD8544_SETYADDR|(starty+cy), LCD_COMMAND);
			__LCD_WRITE(PCD8544_SETXADDR|x, LCD_COMMAND);
			
			for(int cx=0; cx<sx; cx++)
				__LCD_WRITE(bitmapbyte(cx+(cy*sx), bitmap), LCD_DATA);
			
		}
		
		__LCD_WRITE(PCD8544_SETYADDR, LCD_COMMAND);
		__LCD_WRITE(PCD8544_SETXADDR, LCD_COMMAND);
	}
	
}

void LCD5110_clrScr(void){
	if(!__sleep){
		__LCD_WRITE(PCD8544_SETYADDR, LCD_COMMAND);
		__LCD_WRITE(PCD8544_SETXADDR, LCD_COMMAND);
		
		for(int c=0; c<504; c++)
			__LCD_WRITE(0x00, LCD_DATA);
		
	}
	
	
}

void LCD5110_clrRow(int row, int start_x, int end_x){
	
	if(!__sleep){
		__LCD_WRITE(PCD8544_SETYADDR|row, LCD_COMMAND);
		__LCD_WRITE(PCD8544_SETXADDR|start_x, LCD_COMMAND);
		
		for(int c=start_x; c<=end_x; c++)
			__LCD_WRITE(0x00, LCD_DATA);
		
		__LCD_WRITE(PCD8544_SETYADDR, LCD_COMMAND);
		__LCD_WRITE(PCD8544_SETXADDR, LCD_COMMAND);
	}
	
}

void LCD5110_invert(unsigned char mode){

		if(!__sleep){
			if(mode == True)
				__LCD_WRITE(PCD8544_DISPLAYCONTROL|PCD8544_DISPLAYINVVIDEO, LCD_COMMAND);
			else
				__LCD_WRITE(PCD8544_DISPLAYCONTROL|PCD8544_DISPLAYNORMAL, LCD_COMMAND);
		}
	
	
}

void LCD5110_invertText(unsigned char mode){
	if(mode==True)
		cfont.inverted=1;
	else
		cfont.inverted=0;
	
}

void LCD5110_setFont(unsigned char *font){
	
	cfont.font = font;
	cfont.x_size = fontbyte(0);
	cfont.y_size = fontbyte(1);
	cfont.offset = fontbyte(2);
	cfont.numchars = fontbyte(3);
	cfont.inverted = 0;

}

void LCD5110_enableSleep(void){
	__sleep = True;
	__LCD_WRITE(PCD8544_SETYADDR, LCD_COMMAND);
	__LCD_WRITE(PCD8544_SETXADDR, LCD_COMMAND);
	
	for(int b=0; b<504; b++)
		__LCD_WRITE(0x00, LCD_DATA);
	
	__LCD_WRITE(PCD8544_FUNCTIONSET|PCD8544_POWERDOWN, LCD_COMMAND);
	
}


void LCD5110_disableSleep(void){
	
	__LCD_WRITE(PCD8544_FUNCTIONSET|PCD8544_EXTENDEDINSTRUCTION, LCD_COMMAND);
	__LCD_WRITE(PCD8544_SETVOP|__contrast, LCD_COMMAND);
	__LCD_WRITE(PCD8544_SETTEMPCOEF|LCD_TEMP, LCD_COMMAND);
	__LCD_WRITE(PCD8544_SETBIASSYS|LCD_BIAS, LCD_COMMAND);
	__LCD_WRITE(PCD8544_FUNCTIONSET, LCD_COMMAND);
	__LCD_WRITE(PCD8544_DISPLAYCONTROL|PCD8544_DISPLAYNORMAL, LCD_COMMAND);
	__sleep = False;
	
	
}



const unsigned char SmallFont[]  =
{
0x06, 0x08, 0x20, 0x5f,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00,   // sp
0x00, 0x00, 0x00, 0x2f, 0x00, 0x00,   // !
0x00, 0x00, 0x07, 0x00, 0x07, 0x00,   // "
0x00, 0x14, 0x7f, 0x14, 0x7f, 0x14,   // #
0x00, 0x24, 0x2a, 0x7f, 0x2a, 0x12,   // $
0x00, 0x23, 0x13, 0x08, 0x64, 0x62,   // %
0x00, 0x36, 0x49, 0x55, 0x22, 0x50,   // &
0x00, 0x00, 0x05, 0x03, 0x00, 0x00,   // '
0x00, 0x00, 0x1c, 0x22, 0x41, 0x00,   // (
0x00, 0x00, 0x41, 0x22, 0x1c, 0x00,   // )
0x00, 0x14, 0x08, 0x3E, 0x08, 0x14,   // *
0x00, 0x08, 0x08, 0x3E, 0x08, 0x08,   // +
0x00, 0x00, 0x00, 0xA0, 0x60, 0x00,   // ,
0x00, 0x08, 0x08, 0x08, 0x08, 0x08,   // -
0x00, 0x00, 0x60, 0x60, 0x00, 0x00,   // .
0x00, 0x20, 0x10, 0x08, 0x04, 0x02,   // /

0x00, 0x3E, 0x51, 0x49, 0x45, 0x3E,   // 0
0x00, 0x00, 0x42, 0x7F, 0x40, 0x00,   // 1
0x00, 0x42, 0x61, 0x51, 0x49, 0x46,   // 2
0x00, 0x21, 0x41, 0x45, 0x4B, 0x31,   // 3
0x00, 0x18, 0x14, 0x12, 0x7F, 0x10,   // 4
0x00, 0x27, 0x45, 0x45, 0x45, 0x39,   // 5
0x00, 0x3C, 0x4A, 0x49, 0x49, 0x30,   // 6
0x00, 0x01, 0x71, 0x09, 0x05, 0x03,   // 7
0x00, 0x36, 0x49, 0x49, 0x49, 0x36,   // 8
0x00, 0x06, 0x49, 0x49, 0x29, 0x1E,   // 9
0x00, 0x00, 0x36, 0x36, 0x00, 0x00,   // :
0x00, 0x00, 0x56, 0x36, 0x00, 0x00,   // ;
0x00, 0x08, 0x14, 0x22, 0x41, 0x00,   // <
0x00, 0x14, 0x14, 0x14, 0x14, 0x14,   // =
0x00, 0x00, 0x41, 0x22, 0x14, 0x08,   // >
0x00, 0x02, 0x01, 0x51, 0x09, 0x06,   // ?

0x00, 0x32, 0x49, 0x59, 0x51, 0x3E,   // @
0x00, 0x7C, 0x12, 0x11, 0x12, 0x7C,   // A
0x00, 0x7F, 0x49, 0x49, 0x49, 0x36,   // B
0x00, 0x3E, 0x41, 0x41, 0x41, 0x22,   // C
0x00, 0x7F, 0x41, 0x41, 0x22, 0x1C,   // D
0x00, 0x7F, 0x49, 0x49, 0x49, 0x41,   // E
0x00, 0x7F, 0x09, 0x09, 0x09, 0x01,   // F
0x00, 0x3E, 0x41, 0x49, 0x49, 0x7A,   // G
0x00, 0x7F, 0x08, 0x08, 0x08, 0x7F,   // H
0x00, 0x00, 0x41, 0x7F, 0x41, 0x00,   // I
0x00, 0x20, 0x40, 0x41, 0x3F, 0x01,   // J
0x00, 0x7F, 0x08, 0x14, 0x22, 0x41,   // K
0x00, 0x7F, 0x40, 0x40, 0x40, 0x40,   // L
0x00, 0x7F, 0x02, 0x0C, 0x02, 0x7F,   // M
0x00, 0x7F, 0x04, 0x08, 0x10, 0x7F,   // N
0x00, 0x3E, 0x41, 0x41, 0x41, 0x3E,   // O

0x00, 0x7F, 0x09, 0x09, 0x09, 0x06,   // P
0x00, 0x3E, 0x41, 0x51, 0x21, 0x5E,   // Q
0x00, 0x7F, 0x09, 0x19, 0x29, 0x46,   // R
0x00, 0x46, 0x49, 0x49, 0x49, 0x31,   // S
0x00, 0x01, 0x01, 0x7F, 0x01, 0x01,   // T
0x00, 0x3F, 0x40, 0x40, 0x40, 0x3F,   // U
0x00, 0x1F, 0x20, 0x40, 0x20, 0x1F,   // V
0x00, 0x3F, 0x40, 0x38, 0x40, 0x3F,   // W
0x00, 0x63, 0x14, 0x08, 0x14, 0x63,   // X
0x00, 0x07, 0x08, 0x70, 0x08, 0x07,   // Y
0x00, 0x61, 0x51, 0x49, 0x45, 0x43,   // Z
0x00, 0x00, 0x7F, 0x41, 0x41, 0x00,   // [
0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55,   // Backslash (Checker pattern)
0x00, 0x00, 0x41, 0x41, 0x7F, 0x00,   // ]
0x00, 0x04, 0x02, 0x01, 0x02, 0x04,   // ^
0x00, 0x40, 0x40, 0x40, 0x40, 0x40,   // _

0x00, 0x00, 0x03, 0x05, 0x00, 0x00,   // `
0x00, 0x20, 0x54, 0x54, 0x54, 0x78,   // a
0x00, 0x7F, 0x48, 0x44, 0x44, 0x38,   // b
0x00, 0x38, 0x44, 0x44, 0x44, 0x20,   // c
0x00, 0x38, 0x44, 0x44, 0x48, 0x7F,   // d
0x00, 0x38, 0x54, 0x54, 0x54, 0x18,   // e
0x00, 0x08, 0x7E, 0x09, 0x01, 0x02,   // f
0x00, 0x18, 0xA4, 0xA4, 0xA4, 0x7C,   // g
0x00, 0x7F, 0x08, 0x04, 0x04, 0x78,   // h
0x00, 0x00, 0x44, 0x7D, 0x40, 0x00,   // i
0x00, 0x40, 0x80, 0x84, 0x7D, 0x00,   // j
0x00, 0x7F, 0x10, 0x28, 0x44, 0x00,   // k
0x00, 0x00, 0x41, 0x7F, 0x40, 0x00,   // l
0x00, 0x7C, 0x04, 0x18, 0x04, 0x78,   // m
0x00, 0x7C, 0x08, 0x04, 0x04, 0x78,   // n
0x00, 0x38, 0x44, 0x44, 0x44, 0x38,   // o

0x00, 0xFC, 0x24, 0x24, 0x24, 0x18,   // p
0x00, 0x18, 0x24, 0x24, 0x18, 0xFC,   // q
0x00, 0x7C, 0x08, 0x04, 0x04, 0x08,   // r
0x00, 0x48, 0x54, 0x54, 0x54, 0x20,   // s
0x00, 0x04, 0x3F, 0x44, 0x40, 0x20,   // t
0x00, 0x3C, 0x40, 0x40, 0x20, 0x7C,   // u
0x00, 0x1C, 0x20, 0x40, 0x20, 0x1C,   // v
0x00, 0x3C, 0x40, 0x30, 0x40, 0x3C,   // w
0x00, 0x44, 0x28, 0x10, 0x28, 0x44,   // x
0x00, 0x1C, 0xA0, 0xA0, 0xA0, 0x7C,   // y
0x00, 0x44, 0x64, 0x54, 0x4C, 0x44,   // z
0x00, 0x00, 0x10, 0x7C, 0x82, 0x00,   // {
0x00, 0x00, 0x00, 0xFF, 0x00, 0x00,   // |
0x00, 0x00, 0x82, 0x7C, 0x10, 0x00,   // }
0x00, 0x00, 0x06, 0x09, 0x09, 0x06    // ~ (Degrees)
};


const unsigned char MediumNumbers[]  =
{
0x0c, 0x10, 0x2d, 0x0d,
0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x01, 0x00, 0x00,   // -
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00,   // .
0x00, 0x00, 0x02, 0x86, 0x86, 0x86, 0x86, 0x86, 0x86, 0x02, 0x00, 0x00, 0x00, 0x00, 0x81, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0x81, 0x00, 0x00,   // /
0x00, 0xfc, 0x7a, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x7a, 0xfc, 0x00, 0x00, 0x7e, 0xbc, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xbc, 0x7e, 0x00,   // 0
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x7e, 0x00,   // 1
0x00, 0x00, 0x02, 0x86, 0x86, 0x86, 0x86, 0x86, 0x86, 0x7a, 0xfc, 0x00, 0x00, 0x7e, 0xbd, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0x81, 0x00, 0x00,   // 2
0x00, 0x00, 0x02, 0x86, 0x86, 0x86, 0x86, 0x86, 0x86, 0x7a, 0xfc, 0x00, 0x00, 0x00, 0x81, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xbd, 0x7e, 0x00,   // 3
0x00, 0xfc, 0x78, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x78, 0xfc, 0x00, 0x00, 0x00, 0x01, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x3d, 0x7e, 0x00,   // 4
0x00, 0xfc, 0x7a, 0x86, 0x86, 0x86, 0x86, 0x86, 0x86, 0x02, 0x00, 0x00, 0x00, 0x00, 0x81, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xbd, 0x7e, 0x00,   // 5
0x00, 0xfc, 0x7a, 0x86, 0x86, 0x86, 0x86, 0x86, 0x86, 0x02, 0x00, 0x00, 0x00, 0x7e, 0xbd, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xbd, 0x7e, 0x00,   // 6
0x00, 0x00, 0x02, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x7a, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x7e, 0x00,   // 7
0x00, 0xfc, 0x7a, 0x86, 0x86, 0x86, 0x86, 0x86, 0x86, 0x7a, 0xfc, 0x00, 0x00, 0x7e, 0xbd, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xbd, 0x7e, 0x00,   // 8
0x00, 0xfc, 0x7a, 0x86, 0x86, 0x86, 0x86, 0x86, 0x86, 0x7a, 0xfc, 0x00, 0x00, 0x00, 0x81, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xbd, 0x7e, 0x00,   // 9
};

const unsigned char BigNumbers[]  =
{
0x0e, 0x18, 0x2d, 0x0d,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,   // -
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0xe0, 0xe0, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00,   // .
0x00, 0x00, 0x02, 0x06, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x06, 0x02, 0x00, 0x00, 0x00, 0x00, 0x10, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x10, 0x00, 0x00, 0x00, 0x00, 0x80, 0xc0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xc0, 0x80, 0x00, 0x00,   // /
0x00, 0xfc, 0xfa, 0xf6, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0xf6, 0xfa, 0xfc, 0x00, 0x00, 0xef, 0xc7, 0x83, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x83, 0xc7, 0xef, 0x00, 0x00, 0x7f, 0xbf, 0xdf, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xdf, 0xbf, 0x7f, 0x00,   // 0
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0xf8, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x83, 0xc7, 0xef, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x3f, 0x7f, 0x00,   // 1
0x00, 0x00, 0x02, 0x06, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0xf6, 0xfa, 0xfc, 0x00, 0x00, 0xe0, 0xd0, 0xb8, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x3b, 0x17, 0x0f, 0x00, 0x00, 0x7f, 0xbf, 0xdf, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xc0, 0x80, 0x00, 0x00,   // 2
0x00, 0x00, 0x02, 0x06, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0xf6, 0xfa, 0xfc, 0x00, 0x00, 0x00, 0x10, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0xbb, 0xd7, 0xef, 0x00, 0x00, 0x00, 0x80, 0xc0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xdf, 0xbf, 0x7f, 0x00,   // 3
0x00, 0xfc, 0xf8, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0xf8, 0xfc, 0x00, 0x00, 0x0f, 0x17, 0x3b, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0xbb, 0xd7, 0xef, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x3f, 0x7f, 0x00,   // 4
0x00, 0xfc, 0xfa, 0xf6, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x06, 0x02, 0x00, 0x00, 0x00, 0x0f, 0x17, 0x3b, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0xb8, 0xd0, 0xe0, 0x00, 0x00, 0x00, 0x80, 0xc0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xdf, 0xbf, 0x7f, 0x00,   // 5
0x00, 0xfc, 0xfa, 0xf6, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x06, 0x02, 0x00, 0x00, 0x00, 0xef, 0xd7, 0xbb, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0xb8, 0xd0, 0xe0, 0x00, 0x00, 0x7f, 0xbf, 0xdf, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xdf, 0xbf, 0x7f, 0x00,   // 6
0x00, 0x00, 0x02, 0x06, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0xf6, 0xfa, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x83, 0xc7, 0xef, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x3f, 0x7f, 0x00,   // 7
0x00, 0xfc, 0xfa, 0xf6, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0xf6, 0xfa, 0xfc, 0x00, 0x00, 0xef, 0xd7, 0xbb, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0xbb, 0xd7, 0xef, 0x00, 0x00, 0x7f, 0xbf, 0xdf, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xdf, 0xbf, 0x7f, 0x00,   // 8
0x00, 0xfc, 0xfa, 0xf6, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0xf6, 0xfa, 0xfc, 0x00, 0x00, 0x0f, 0x17, 0x3b, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0xbb, 0xd7, 0xef, 0x00, 0x00, 0x00, 0x80, 0xc0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xdf, 0xbf, 0x7f, 0x00,   // 9
};
