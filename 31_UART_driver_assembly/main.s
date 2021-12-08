; UART0 and UART2 ==> Virtual COM port
; JP4 and JP5 contols
;JP4 and JP5 should be CAN mode so it will use UART2 for Virtual COM port
; UART2 will be used 

; PD4	U2Rx
; PD5	U2Tx

;SYS CLOCK EN
SYSCTRL_BASE				EQU		0x400FE000
RCGCGPIO_OFFSET				EQU		0x608
RCGCUART_OFFSET				EQU		0x618
SYSCTRL_RCGCGPIO_R			EQU		SYSCTRL_BASE + RCGCGPIO_OFFSET
SYSCTRL_RCGCUART_R			EQU		SYSCTRL_BASE + RCGCUART_OFFSET	
	
;PORT D
GPIOD_BASE 					EQU		0x4005B000
GPIOD_DEN_OFFSET			EQU		0x51C
GPIOD_DEN_R					EQU		GPIOD_BASE + GPIOD_DEN_OFFSET
	
GPIOD_AFSEL_OFFSET			EQU		0x420
GPIOD_AFSEL_R				EQU		GPIOD_BASE + GPIOD_AFSEL_OFFSET

GPIOD_PCTL_OFFSET			EQU		0x52C
GPIOD_PCTL_R				EQU		GPIOD_BASE + GPIOD_PCTL_OFFSET


GPIOD_EN					EQU		0x08	;RCGCGPIO en
UART2_EN					EQU		0x04	;RCGCUART en

;UART2
UART2_BASE					EQU		0x4000E000
UART2_DR_OFFSET				EQU		0x000
UART2_DR_R					EQU		UART2_BASE + UART2_DR_OFFSET ;data register	

UART2_FR_OFFSET				EQU		0x018
UART2_FR_R					EQU		UART2_BASE + UART2_FR_OFFSET ;flag register

UART2_IBRD_OFFSET			EQU		0x024
UART2_IBRD_R				EQU		UART2_BASE + UART2_IBRD_OFFSET ;integer part of the baud-rate divisor value

UART2_FBRD_OFFSET			EQU		0x028
UART2_FBRD_R				EQU		UART2_BASE + UART2_FBRD_OFFSET ;fractional part of the baud-rate divisor value
	
UART2_LCTRL_OFFSET			EQU		0x02C
UART2_LCTRL_R				EQU		UART2_BASE + UART2_LCTRL_OFFSET ;line control register
	
UART2_CTRL_OFFSET			EQU		0x030
UART2_CTRL_R				EQU		UART2_BASE + UART2_CTRL_OFFSET ;uart control register

UART2_IFLS_OFFSET			EQU		0x034
UART2_IFLS_R				EQU		UART2_BASE + UART2_IFLS_OFFSET ;uart interrupt fifo select register

UART2_IM_OFFSET				EQU		0x038
UART2_IM_R					EQU		UART2_BASE + UART2_IM_OFFSET ;IRQ mask set/clear register

UART2_RIS_OFFSET			EQU		0x03C
UART2_RIS_R					EQU		UART2_BASE + UART2_RIS_OFFSET ;raw interrupt status register

UART2_ICR_OFFSET			EQU		0x044
UART2_ICR_R					EQU		UART2_BASE + UART2_ICR_OFFSET ;interrupt clear register


;constants
UART_FR_RXFE 				EQU		0x00000010 ;is recv fifo empty FR register 4th bit
UART_LCTRL_WLEN_8			EQU		0x00000060 ;8 bit word size 0x3<<5
UART_LCTRL_FEN				EQU		0x00000010 ;enable the uart fifo
UART_CTRL_UARTEN			EQU		0x00000001 ;enable UART
UART_IM_RTIM				EQU		0x00000040 ;recv uart fifo timeout

;
CR		EQU		0x0D ;Carriage return
BS		EQU		0x08 ;backspace
LF		EQU		0x0A ;Line Feed
ESC		EQU		0x1B ;Escape
SPA		EQU		0x20 ;Space
DEL 	EQU		0x7F ;Delete
	


	AREA |.text|, CODE, READONLY, ALIGN=2
	THUMB
	ENTRY
	EXPORT __main
		

__main

	BL 		UART_Init
	
	MOV		R4, #'A'

lp0	MOV		R0, R4
	BL		UART_WRITE
	ADD		R4, R4, #1
	CMP		R4, #'Z'
	BLS		lp0
	BL		newline
	MOV		R0, #'!'
	BL		UART_WRITE
	
	

newline
	PUSH	{LR}
	
	MOV		R0, #CR
	BL		UART_WRITE
	MOV 	R0, #LF
	BL		UART_WRITE
	
	POP		{PC}

UART_Init
	
	PUSH	{LR}
	
	;SYSCTL->RCGCUART	|= UART2_EN;
	LDR		R1,	=SYSCTRL_RCGCUART_R	
	LDR		R0, [R1]
	ORR		R0, #UART2_EN
	STR		R0, [R1]
	
	;SYSCTL->RCGCGPIO	|= GPIOD_EN;
	LDR		R1,	=SYSCTRL_RCGCGPIO_R	
	LDR		R0, [R1]
	ORR		R0, #GPIOD_EN
	STR		R0, [R1]
	
	;set uart pin alternate func
	;GPIOD->AFSEL	|= 0x30;
	LDR		R1, =GPIOD_AFSEL_R
	LDR		R0, [R1]
	ORR		R0, #0x30
	STR		R0, [R1]
	
	;Digitally enable 
	;GPIOD->DEN			|= 0x30;	PD4 and PD5
	LDR		R1, =GPIOD_DEN_R
	LDR		R0, [R1]
	ORR		R0, #0x30
	STR		R0, [R1]
	
	;GPIOD->PCTL		&=~0x00FF0000 ; Clear bit 
	;GPIOD->PCTL		&= 0x00110000 ; First functionality Uart2
	LDR		R1, =GPIOD_PCTL_R
	LDR		R0, [R1]
	BIC		R0, R0, #0x00FF0000
	ADD		R0, R0, #0x00110000
	STR		R0, [R1]
	
	
	;Disable the UART
	LDR		R1, =UART2_CTRL_R
	LDR		R0, [R1]
	BIC		R0, R0, #UART_CTRL_UARTEN
	STR		R0, [R1]
	
	;Configure Baud-Rate
	;16MHZ clk rate and baudrates 9600, 115200 etc
	; 16M / (16 * 115200) = 8.6806
	; BRD = BRDI + BRDF = UARTSysClk / (ClkDiv * Baud Rate)
	LDR		R1, =UART2_IBRD_R
	LDR		R0, [R1]
	MOV		R0, #8
	STR		R0, [R1]
	; Add fraction to the fraction register
	; 0.6806 *64 + 0.5 = 44.0584
	;UARTFBRD[DIVFRAC] = integer(BRDF * 64 + 0.5)
	LDR		R1, =UART2_FBRD_R
	LDR		R0, [R1]
	MOV		R0, #44
	STR		R0, [R1]
	
	;Word Lenght Configuration and FIFO enable
	LDR		R1, =UART2_LCTRL_R
	LDR		R0, [R1]
	BIC		R0, R0, #0xFF ; clears
	ADD		R0, R0, #(UART_LCTRL_WLEN_8 + UART_LCTRL_FEN)
	STR		R0, [R1]
	
	;Enable the UART
	LDR		R1, =UART2_CTRL_R
	LDR		R0, [R1]
	ORR		R0, R0, #UART_CTRL_UARTEN
	STR		R0, [R1]
	
	POP		{PC}
	
UART_READ
	LDR 	R1, =UART2_FR_R
	
lp1	LDR		R2, [R1]
	ANDS		R2, #0x0010 ;RXFE
	BNE		lp1			;FIFO is empty
	
	LDR 	R1, =UART2_DR_R
	LDR		R0, [R1]
	BX		LR

UART_WRITE
	LDR 	R1, =UART2_FR_R
	
lp2	LDR		R2, [R1]
	ANDS		R2, #0x0020 ;TXFE
	BNE		lp2			;FIFO is full
	
	LDR 	R1, =UART2_DR_R
	STR		R0, [R1]
	BX		LR


	ALIGN
	END
