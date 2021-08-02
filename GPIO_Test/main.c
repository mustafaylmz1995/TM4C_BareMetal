#include "TM4C129.h"                    // Device header


int main(){
	
	SYSCTL->RCGCGPIO |= 0xFFFF;
	
	
	//direction
	GPIOA_AHB->DIR |= 0xFF;
	GPIOB_AHB->DIR |= 0xFF;
	GPIOC_AHB->DIR |= 0xFF;
	GPIOD_AHB->DIR |= 0xFF;
	GPIOE_AHB->DIR |= 0xFF;
	GPIOF_AHB->DIR |= 0xFF;
	GPIOG_AHB->DIR |= 0xFF;
	GPIOH_AHB->DIR |= 0xFF;
	GPIOJ_AHB->DIR |= 0xFF;
	
	GPIOK->DIR |= 0xFF;
	GPIOL->DIR |= 0xFF;
	GPIOM->DIR |= 0xFF;
	GPION->DIR |= 0xFF;
	GPIOP->DIR |= 0xFF;	
	GPIOQ->DIR |= 0xFF;

	
	
	//digen
	GPIOA_AHB->DEN |= 0xFF;
	GPIOB_AHB->DEN |= 0xFF;
	GPIOC_AHB->DEN |= 0xFF;
	GPIOD_AHB->DEN |= 0xFF;
	GPIOE_AHB->DEN |= 0xFF;
	GPIOF_AHB->DEN |= 0xFF;
	GPIOG_AHB->DEN |= 0xFF;
	GPIOH_AHB->DEN |= 0xFF;
	GPIOJ_AHB->DEN |= 0xFF;
	
	GPIOK->DEN |= 0xFF;
	GPIOL->DEN |= 0xFF;
	GPIOM->DEN |= 0xFF;
	GPION->DEN |= 0xFF;
	GPIOP->DEN |= 0xFF;	
	GPIOQ->DEN |= 0xFF;
	
	//data
	GPIOA_AHB->DATA |= 0xFF;
	GPIOB_AHB->DATA |= 0xFF;
	GPIOC_AHB->DATA |= 0xFF;
	GPIOD_AHB->DATA |= 0xFF;
	GPIOE_AHB->DATA |= 0xFF;
	GPIOF_AHB->DATA |= 0xFF;
	GPIOG_AHB->DATA |= 0xFF;
	GPIOH_AHB->DATA |= 0xFF;
	GPIOJ_AHB->DATA |= 0xFF;
	
	GPIOK->DATA |= 0xFF;
	GPIOL->DATA |= 0xFF;
	GPIOM->DATA |= 0xFF;
	GPION->DATA |= 0xFF;
	GPIOP->DATA |= 0xFF;	
	GPIOQ->DATA |= 0xFF;
	
	
	
}