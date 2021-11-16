#include "SevSegment.h"
#include "delay.h"
#include "TM4C129.h"                    // Device header

void GPIO_Init_LED(void){
	SYSCTL->RCGCGPIO = (1U<<11); //M Port
	GPIOM->DIR = (A|B|C_val|D|E|F|G); // Output;	
	GPIOM->DEN = (A|B|C_val|D|E|F|G); // Enable;	
}

void LED_on(unsigned int val){
	GPIOM->DATA |= val;
}

void LED_off(unsigned int val){
	GPIOM->DATA &= ~val;
}

void LED_check(void){
	LED_on(A);
	delay(500*ms);
	LED_off(A);
	delay(500*ms);
	LED_on(B);
	delay(500*ms);
	LED_off(B);
	delay(500*ms);
	LED_on(C_val);
	delay(500*ms);
	LED_off(C_val);
	delay(500*ms);
	LED_on(D);
	delay(500*ms);
	LED_off(D);
	delay(500*ms);
	LED_on(E);
	delay(500*ms);
	LED_off(E);
	delay(500*ms);
	LED_on(F);
	delay(500*ms);
	LED_off(F);
	delay(500*ms);
	LED_on(G);
	delay(500*ms);
	LED_off(G);
	delay(500*ms);
	LED_on((A|B|C_val|D|E|F|G));
	delay(500*ms);
	LED_clr();
	delay(500*ms);
}



void LED_clr(void){
	
	LED_off((A|B|C_val|D|E|F|G));
}

void LED_print_check(void){
	
	delay(1*ms*ms);
	LED_print(0);
	delay(1*ms*ms);
	LED_print(1);
	delay(1*ms*ms);
	LED_print(2);
	delay(1*ms*ms);
	LED_print(3);
	delay(1*ms*ms);
	LED_print(4);
	delay(1*ms*ms);
	LED_print(5);
	delay(1*ms*ms);
	LED_print(6);
	delay(1*ms*ms);
	LED_print(7);
	delay(1*ms*ms);
	LED_print(8);
	delay(1*ms*ms);
	LED_print(9);
	delay(1*ms*ms);
	LED_print(30);

}


void LED_print(unsigned int t){
	
	LED_clr();
	switch(t){
		case 0:
			LED_on(zero);
			break;
		case 1:
			LED_on(one);
			break;
		case 2:
			LED_on(two);
			break;
		case 3:
			LED_on(three);
			break;
		case 4:
			LED_on(four);
			break;
		case 5:
			LED_on(five);
			break;
		case 6:
			LED_on(six);
			break;
		case 7:
			LED_on(seven);
			break;
		case 8:
			LED_on(eight);
			break;
		case 9:
			LED_on(nine);
			break;
		default:
			LED_on(error);
			break;
		
	}
	
}