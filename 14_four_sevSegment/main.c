#include "TM4C129.h"                    // Device header
#include "delay.h"

#define A	(1U<<0)
#define B	(1U<<1)
#define C	(1U<<2)
#define D	(1U<<3)
#define E	(1U<<4)
#define F	(1U<<5)
#define G	(1U<<6)
#define dot (1U<<7)

#define zero	(A|B|C|D|E|F)
#define one 	(B|C)
#define two 	(A|B|D|E|G)
#define three	(A|B|C|D|G)
#define four	(B|C|F|G)
#define five	(A|C|D|F|G)
#define six		(A|C|D|E|F|G)
#define seven	(A|B|C)
#define eight	(A|B|C|D|E|F|G)
#define nine	(A|B|C|D|F|G)
#define error (A|D|E|F|G)


unsigned int arr[13]={zero, one, two, three, four, five, six, seven, eight, nine, zero, one, two};

void GPIO_Init_LED(void){
	SYSCTL->RCGCGPIO |= (1U<<11); //M Port
	GPIOM->DIR = (A|B|C|D|E|F|G|dot); // Output;	
	GPIOM->DEN = (A|B|C|D|E|F|G|dot); // Enable;	

	SYSCTL->RCGCGPIO |= (1U<<12); //N Port
	GPION->DIR = (A|B|C|D); // Output;
	GPION->DEN = (A|B|C|D); // Enable;
	
}

void LED_clear(void){
	GPION->DATA &= ~(A|B|C|D);
	GPIOM->DATA &= ~(A|B|C|D|E|F|G|dot);
}

void LED_off(unsigned int LEDnumber, unsigned int val){
	LED_clear();
	GPION->DATA |= (1U<<LEDnumber);
	GPIOM->DATA |= val;
}

void LED_on(unsigned int LEDnumber, unsigned int val){
	LED_clear();
	GPION->DATA |= (1U<<LEDnumber);
	GPIOM->DATA |= ~val;
}



int main(){
	
	GPIO_Init_LED();
	int j = 0, i= 0;
	while(1){
		
		if( j == 20) {
			i++;
			if(i==10)
				i = 0;
			j=0;
		}
		
		LED_on(0, arr[i]);
		delay(15*ms);
		LED_on(1, arr[i+1]);
		delay(15*ms);
		LED_on(2, arr[i+2]);
		delay(15*ms);
		LED_on(3, arr[i+3]);
		delay(15*ms);
		j++;
	}
	return 0;
	
}
