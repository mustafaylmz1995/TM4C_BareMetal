SYS_RCGCGPIO_PORT	EQU	0x400FE608
GPIODIR_PORTN		EQU 0x40064400	
GPIODEN_PORTN		EQU 0x4006451C
	
GPIODATA_PORTN		EQU	0x400643FC ;shift 2 bit left ;3FC all opened
	
GPIOPUR_PORTJ		EQU	0x40060510
GPIOLOCK_PORTJ		EQU 0x40060520
GPIOCR_PORTJ		EQU	0x40060524

GPIO_LOCK_KEY		EQU 0x4C4F434B
	
LED0				EQU 0x02
LED1				EQU 0x01
SEC_DIV_FIVE		EQU 1066666 ; 1sec/5
	
GPIODIR_PORTJ		EQU 0x40060400	
GPIODEN_PORTJ		EQU 0x4006051C
GPIODATA_PORTJ		EQU 0x400603FC ;base address	


	AREA	|.text|, CODE, READONLY, ALIGN=4 
		THUMB
		EXPORT __main
			
__main
	BL	GPIO_Init			

Loop
	ldr R0, =SEC_DIV_FIVE
	BL	Delay
	BL 	Input
	cmp R0 , #0x03
	BEQ	Switch_On
	BL 	Output
	B	Loop

Switch_On
	mov R0, #LED1
	BL Output
	B  Loop

Output	
	ldr R1, =GPIODATA_PORTN
	str R0, [R1]
	BX	LR

Input
	ldr	R1, =GPIODATA_PORTJ
	ldr R0, [R1]
	and	R0, R0, #0x01 ;0000 0001 0. bit pressed ; 0. ve 1. bit icin 0x03
	BX	LR

Delay
	subs R0, R0, #1
	BNE Delay
	BX	LR

GPIO_Init
	ldr R1, =SYS_RCGCGPIO_PORT
	ldr	R0, [R1]
	orr	R0, R0, #0x1100 ;0001 0001 0000 0000 12. bit N portu 8. bit J portu
	str R0, [R1]
	
	ldr R1, =GPIOPUR_PORTJ
	ldr	R0, [R1]
	orr	R0, R0, #0x01 ; 0000 0001 0. bit pull-up enable ; 0. ve 1. bit icin 0x03
	str R0, [R1]
	
	ldr R1, =GPIOLOCK_PORTJ	;lock 
	ldr R0, =GPIO_LOCK_KEY
	str R0, [R1]

	ldr	R1, =GPIOCR_PORTJ	;commit
	mov R0, #0xFF
	str R0, [R1]
	
	ldr R1, =GPIODIR_PORTN
	ldr	R0, [R1]
	orr	R0, R0, #0x03 ; 0000 0011 0. ve 1. bit output
	str R0, [R1]

	ldr R1, =GPIODIR_PORTJ
	ldr	R0, [R1]
	and	R0, R0, #0xFE ; 1111 1110 0. bit input  ; 0. ve 1. bit icin 0xFC
	str R0, [R1]

	ldr R1, =GPIODEN_PORTN
	ldr	R0, [R1]
	orr	R0, R0, #0x03 ; 0000 0011 0. ve 1. bit dig_en
	str R0, [R1]

	ldr R1, =GPIODEN_PORTJ
	ldr	R0, [R1]
	orr	R0, R0, #0x01 ; 0000 0001 0. bit dig_en ; 0. ve 1. bit icin 0x03
	str R0, [R1]


	BX	LR			
	
	ALIGN
	END