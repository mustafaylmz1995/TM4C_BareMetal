#ifndef __SYSTICK_LIB_H__
#define __SYSTICK_LIB_H__
#include <stdbool.h>

void SysTick_Init(unsigned int us);
bool SysTick_Timeout(void);

#endif //__SYSTICK_LIB_H__

