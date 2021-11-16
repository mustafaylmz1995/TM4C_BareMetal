#include "TM4C129.h"                    // Device header

#include "SevSegment.h"//PORT M 
#include "delay.h"

int main(){
	
	GPIO_Init_LED();
	LED_check();
	LED_check();
	
	LED_print_check();
	LED_print_check();
	LED_print_check();
	
	
}
