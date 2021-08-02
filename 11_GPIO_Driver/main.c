//#include "TM4C129.h"                    // Device header
#include "GPIO_Init.h"

uint32_t const sw1 = 0x01;
uint32_t const sw2 = 0x02;

uint32_t sw_state;


int main(){
	
	GPIO_start();
	
//	while(1){
//		sw_state = SW_INPUT();
//		
//		switch(sw_state){			
//			case sw1:
//				Blink(1, 100);
//			break;
//			case sw2:
//				Blink(2, 100);
//			break;
//			case (sw1 + sw2):
//				Blink(3, 100);
//			break;
//			default:
//				Blink(4, 100);
//			break;
//			
//		}
//		
//	}

	
	return 0;
}










