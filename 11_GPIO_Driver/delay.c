#include "delay.h"


void delay(uint32_t time){
	uint32_t volatile delay=0;
	while(delay<time)
		++delay;
	
}
