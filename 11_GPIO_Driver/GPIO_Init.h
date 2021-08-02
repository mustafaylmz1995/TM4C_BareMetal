#ifndef __GPIO_INIT_H__
#define __GPIO_INIT_H__


typedef unsigned int uint32_t;

#define LED1 (1U<<1)	//PN1
#define LED2 (1U<<0)	//PN0
#define LED3 (1U<<4)	//PF4
#define LED4 (1U<<0)	//PF0

#define SW1  (1U<<0)	//PJ0
#define SW2  (1U<<1)	//PJ1

#define KEY	0x4C4F434B

void SYSCTRL_LED_SW_start(void);
void onBoardSW_start(void);
void onBoardLED_start(void);
void GPIO_start(void);

uint32_t SW_INPUT(void);
void LED_On(short int LED);
void LED_Off(short int LED);
void Blink(short int LED, uint32_t MS);

#endif //__GPIO_INIT_H__
