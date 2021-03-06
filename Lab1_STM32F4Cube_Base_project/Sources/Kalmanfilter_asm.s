	AREA myCode, CODE, READONLY
	EXPORT Kalmanfilter_asm
		
;------------------Register Allocation----------------;
; R0 --> Input array base address 					  ;
; R1 --> Output array base address 					  ;
; R2 --> Kalman state base address : q, r, x, p, k 	  ;
; R3 --> Length of input array : int 				  ;
; R4 --> int i										  ;
; R5 --> tmp  										  ;
; S0 --> State variable : x							  ;
; S1 --> q																						;
; S2 --> r																						;
; S3 --> p																						;
; S4 --> k																						;
; S5 --> R0[i]																				;
; S6 --> tmp																					;
; S7 --> 1.0																					;
;-----------------------------------------------------;
Kalmanfilter_asm

		;----------------------INIT-----------------------;
		PUSH {R4-R5, LR}							; save register states
		VLDR S1, [R2]             		; load q
		VLDR S2, [R2, #0X04]		  		; load r
		VLDR S3, [R2, #0X0C]					; load p
		VLDR S4, [R2, #0X10]					; load k
		VLDR S0, [R2, #0X08]					; load x_(i-1) (last state of x)
		VMOV.F32 S7, #1.0							; constant register '1'
		LDR R4, =0x0									; int i = 0
		
loop
		ADD R5, R0, R4								; tmp = R0[i] address
		VLDR S5, [R5]									; Get value of R0[i]
		
		;-----------------Update Filter State--------------;
		VADD.F32 S3, S3, S1						; p = p+q
		VADD.F32 S6, S3, S2						; tmp = p+r
		VDIV.F32 S4, S3, S6						; k = p/tmp
		VSUB.F32 S6, S5, S0						; tmp = input[i] - x_(i-1)
		VMUL.F32 S6, S6, S4						; tmp = tmp * k
		VADD.F32 S0, S0, S6						; x = x_(i-1) + tmp
		VSUB.F32 S6, S7, S4						; tmp = 1 - k
		VMUL.F32 S3, S3, S6						; p = p * tmp
		
		;----------Save value to output array---------------;
		ADD R5, R1, R4								; tmp = R1[i] address
		VSTR.F32 S0, [R5]							; Output[i] = S0
		ADD R4, R4, #0x04							; increment i
		
		LSR R5, R4, #0x02							; divide i by 4 to normalize to index
		SUBS R5, R5, R3								; subtract and update APSR
		BNE loop											; if i != length (in other words i<Length) loop
		
		VMRS	R5, FPSCR								; copy the Floating Point Status and Control Register into R5 (set flags)
		MOVVS	R0, #-1									; Return -1 if overflow set
		MOVVC R0, #0									; Return 0 if no overflow set
		ANDS	R5, R5, #2							; bitwise AND to extract the divide by zero exception bit
		MOVNE	R0, #-1
		POP {R4-R5, LR}								; Restore state
		
		BX	LR
	
	END