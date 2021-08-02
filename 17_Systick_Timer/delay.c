#include "delay.h"

void delay_ms(unsigned int ms){
	
	for(unsigned int i = 0; i<ms; i++){
		for(unsigned int j = 0; j<3180; ){
			j++;
		}
	}

}

void delay_us(unsigned int us){
	for(unsigned int i = 0; i<us; i++){
		for(unsigned int j = 0; j<3; ){
			j++;
		}
	}
}


