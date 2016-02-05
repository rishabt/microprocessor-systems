	AREA myCode, CODE, READONLY
	IMPORT Kalmanfilter_asm
	IMPORT output_array_base_address
	IMPORT test_status_base_address
	IMPORT kalman_state_base_address
	EXPORT Kalmanfilter_testbench_asm
		
Kalmanfilter_testbench_asm	
	BL	test_general_case
	B		finish
	
	
	
	
	
	
test_general_case

	PUSH {LR}																	; Save return address 
	
	
	LDR R0, =input_array_base_address					; Input array base address (read only)
	LDR R1, =output_array_base_address				; Output array base address (read-write)	
	LDR R3, =array_length_address
	LDR R3, [R3]															; Load array length
	LDR R2, =kalman_state_base_address				; Kalman state base address (q, r, x, p, k) (read-write)
	
	;-----------------Initialize Read-Write Kalman State--------------;
	
	LDR	R4,	=kalman_state_base_address_values	; temp register -> Kalman state values to be stored in SRAM
	LDR	R5, [R4]															; Load temp register R5 with the contents of the kalman_state values
	STR R5, [R2]
	
	LDR	R5, [R4, #0x04]
	STR R5, [R2, #0x04]
	
	LDR	R5, [R4, #0x08]
	STR R5, [R2, #0x08]
	
	LDR	R5, [R4, #0x0C]
	STR R5, [R2, #0x0C]
	
	LDR	R5, [R4, #0x10]
	STR R5, [R2, #0x10]
	;--------------------------------------------------------------------;
	
	
	;-----------------Subroutine Call--------------;
	
	BL	Kalmanfilter_asm
	
	;----------------------------------------------;

	POP {LR}																	; Pop return address 

	LDR 			R0, =test_general_result				; Load predetermined results address
	LDR				R1, =output_array_base_address	; Load output array address
	MOV				R2, #0													; int i
	LDR R6, =array_length_address
	LDR R6, [R6]															; Load array length
loop
	ADD				R3, R2, R0											; R3 = base_predetermined_address + i
	ADD				R4, R2, R1											; R4 = output_array_address + i
	VLDR.F32	S0, [R3]												; S0 = test_results_array[i]
	VLDR.F32	S1, [R4]												; S1 = output_array[i]
	VCMP.F32	S0, S1													; compare both results
	VMRS  APSR_nzcv, FPSCR										; copy FPSCR 		
	BNE	fail_test_general											; check if failed tests
	ADD			R2, R2, #4												; i++
	
	LSR R5, R2, #0x02													; divide i by 4 to normalize to index
	SUBS R5, R5, R6														; subtract i and length and update APSR
	BNE loop																	; if i != length (in other words i<Length) loop
	
	MOV R1, #1
	LDR R0, =test_status_base_address		; Load Test Status Address
	STR	R1,	[R0]												; Set test to success
	BX		LR
	

fail_test_general
	MOV R1, #0
	LDR R0, =test_status_base_address		; Test Status Address
	STR	R1,	[R0]												; Set test status to fail
	BX		LR

	ALIGN
input_array_base_address							DCFS	-1, 0.125, 31.0, 1.0625
kalman_state_base_address_values			DCFS	0.1, 0.1, 0, 0.1, 0
array_length_address									DCD		4
test_general_result										DCFS	0xbf2aaaaa,0xbe2ffffc,0x41990000,0x40feb0e0

finish	
	NOP
	NOP
	
	END
