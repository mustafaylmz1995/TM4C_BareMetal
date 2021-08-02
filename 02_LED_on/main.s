
	AREA	|.text|, CODE, READONLY, ALIGN=4 
		THUMB
		EXPORT __main
			
RCGCGPIO_PORTN		EQU	0x400FE608
GPIODIR_PORTN		EQU 0x40064400	
GPIODEN_PORTN		EQU 0x4006451C
GPIODATA_PORTN		EQU	0x4006400C ;shift 2 bit left ;3FC all opened	
	
__main
	BL	GPIO_Init

Loop
	BL 	LED_On
	B	Loop
	

	
GPIO_Init
	ldr R1, =RCGCGPIO_PORTN
	ldr	R0, [R1]
	orr	R0, R0, #0x1000 ;0001 0000 0000 0000 12. bit N portu
	str R0, [R1]
		
	ldr R1, =GPIODIR_PORTN
	ldr	R0, [R1]
	orr	R0, R0, #0x03 ; 0000 0011 0. ve 1. bit
	str R0, [R1]
	
;	ldr R1, =GPIOAFSEL_PORTN
;	ldr	R0, [R1]
;	and	R0, R0, #0xFC ;1111 1100 0. ve 1. bit 0 olacak
;	str R0, [R1]

	ldr R1, =GPIODEN_PORTN
	ldr	R0, [R1]
	orr	R0, R0, #0x03 ; 0000 0011 0. ve 1. bit
	str R0, [R1]	

	
	BX	LR
	

	
LED_On	
	ldr R1, =GPIODATA_PORTN
	ldr	R0, [R1]
	orr	R0, R0, #0x03 ;0000 0000 0000 0011 0. ve 1. bit
	str R0, [R1]
	BX	LR
	
	ALIGN
	END