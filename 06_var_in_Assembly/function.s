
;----------------------------------------------------
		AREA	DATA, ALIGN=2
amount	SPACE	4 ;each space is byte ; 4byte 32bit
		EXPORT	amount
			
;----------------------------------------------------

;----------------------------------------------------
		AREA	|.text|, CODE, READONLY, ALIGN=2
		THUMB
		EXPORT	Amount_Func
		
Amount_Func
		ldr		R1, =amount
		mov		R0, #89
		str		R0, [R1]
		bx		LR
		
		ALIGN
		END
;----------------------------------------------------