	
	AREA	DATA, ALIGN=2
	IMPORT num
	IMPORT Adder
	
	
	AREA	|.text|, CODE, READONLY, ALIGN=2
	THUMB
	EXPORT __main

__main
	ldr	R1, =num
	mov	R0, #100
	STR	R0, [R1]
	BL	Adder
			
	
	ALIGN
	END
	
	
	