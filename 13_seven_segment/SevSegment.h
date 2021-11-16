#ifndef __SEVSEGMENT_H__
#define __SEVSEGMENT_H__

#define A	(1U<<0)
#define B	(1U<<1)
#define C_val	(1U<<2)
#define D	(1U<<3)
#define E	(1U<<4)
#define F	(1U<<5)
#define G	(1U<<6)

#define zero	(A|B|C_val|D|E|F)
#define one 	(B|C_val)
#define two 	(A|B|D|E|G)
#define three	(A|B|C_val|D|G)
#define four	(B|C_val|F|G)
#define five	(A|C_val|D|F|G)
#define six		(A|C_val|D|E|F|G)
#define seven	(A|B|C_val)
#define eight	(A|B|C_val|D|E|F|G)
#define nine	(A|B|C_val|D|F|G)
#define error (A|D|E|F|G)


void GPIO_Init_LED(void);
void LED_on(unsigned int val);
void LED_off(unsigned int val);
void LED_check(void);

void LED_clr(void);
void LED_print(unsigned int t);
void LED_print_check(void);


#endif //__SEVSEGMENT_H__
