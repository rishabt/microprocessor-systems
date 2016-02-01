	AREA myCode, CODE, READONLY
	IMPORT Kalmanfilter_asm
	IMPORT input_array_base_address
	IMPORT output_array_base_address
	IMPORT kalman_state_base_address
	IMPORT test_status_base_address
		
	EXPORT Kalmanfilter_testbench_asm
		
Kalmanfilter_testbench_asm	
																B	test_general_case
finish_test_general							B test_all_zero_inputs
finish_test_all_zero_inputs			NOP

fail_test_general
	MOV R1, #0
	LDR R0, =test_status_base_address		; Test Status Address
	STR	R1,	[R0]												; Set test status to fail
	B		finish_test_general
	
test_general_case
	LDR R0, =input_array_base_address					; Input array base address
	VMOV.F32 S0, #-1.0
	VSTR.F32 S0, [R0]
	VMOV.F32 S0, #0.125
	VSTR.F32 S0, [R0, #0x04]
	VMOV.F32 S0, #31
	VSTR.F32 S0, [R0, #0x08]
	VMOV.F32 S0, #1.0625
	VSTR.F32 S0, [R0, #0x0C]

	LDR R1, =output_array_base_address				; Output array base address
	
	LDR R2, =kalman_state_base_address				; Kalman state base address
	VMOV.F32 S0, #-15.5					
	VSTR.F32 S0, [R2]							; q
	VMOV.F32 S0, #2.25
	VSTR.F32 S0, [R2, #0x04]			; r
	VMOV.F32 S0, #-0.59375
	VSTR.F32 S0, [R2, #0x08]			; x
	VMOV.F32 S0, #-0.296875
	VSTR.F32 S0, [R2, #0x0C]			; p
	VMOV.F32 S0, #-1.3125
	VSTR.F32 S0, [R2, #0x10]			; q
	
	MOV R3, #0x04									; Load array length
	
	BL	Kalmanfilter_asm
	NOP
	
	LDR 			R0, =test_general_result
	LDR				R1, =output_array_base_address
	VLDR.F32	S0, [R0]
	VLDR.F32	S1, [R1]
	VCMP.F32	S0, S1
	VMRS  APSR_nzcv, FPSCR
	BNE	fail_test_general
	
	VLDR.F32	S0, [R0, #0x04]
	VLDR.F32	S1, [R1, #0x04]
	VCMP.F32	S0, S1
	VMRS  APSR_nzcv, FPSCR
	BNE	fail_test_general
	
	VLDR.F32	S0, [R0, #0x08]
	VLDR.F32	S1, [R1, #0x08]
	VCMP.F32	S0, S1
	VMRS  APSR_nzcv, FPSCR
	BNE	fail_test_general
	
	VLDR.F32	S0, [R0, #0x0C]
	VLDR.F32	S1, [R1, #0x0C]
	VCMP.F32	S0, S1
	VMRS  APSR_nzcv, FPSCR
	BNE	fail_test_general
	
	MOV R1, #1
	LDR R0, =test_status_base_address		; Test Status Address
	STR	R1,	[R0]												; Set test to success
	B		finish_test_general
	
test_all_zero_inputs
	NOP
	NOP
	
test_all_negative_inputs
test_single_input
test_largest_inputs
test_empty_input

	ALIGN
test_general_result	DCFS	0xBF88A2FE,0x3EC146C4,0x42163165,0xC0D7BB18

	END
