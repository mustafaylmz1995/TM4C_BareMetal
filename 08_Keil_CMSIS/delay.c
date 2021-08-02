#include "delay.h"

void wait(unsigned volatile int delay){
	unsigned int a = delay;
	
	while(a){
		a--;
	}
	
}
