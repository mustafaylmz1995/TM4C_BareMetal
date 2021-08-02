#ifndef __KEYPAD_H__
#define __KEYPAD_H__



/*
	IMPORTANT NOTE 
	
	If you use keypad library you need to connect Port A 0 pin Column 0 means 5th pin on the keypad.
	
	PA0 is normally an uart0 port and we need to change jp4 as CAN configuration in order to use this pin.
	JP4 configuration changed from horizontal to vertical configuration 
		
	
	//   C3  C2  C1  C0  R3  R2  R1  R0
	// ___8___7___6___5___4___3___2___1___		Keypad
	//   PA6 PA4 PA2 PA0 PA7 PA5 PA3 PA1
	
	//  
	//   PA7 PA6 PA5 PA4 PA3 PA2 PA1 PA0
	// ___7___6___5___4___3___2___1___0___		MCU
	//    R3  C3  R2  C2  R1  C1  R0  C0


*/



void GPIO_Init_KeyPad(void);
unsigned char getKey(void);


#endif //__KEYPAD_H__

