	AREA myCode, CODE, READONLY
	IMPORT Kalmanfilter_asm
	EXPORT Kalmanfilter_testbench_asm
		
Kalmanfilter_testbench_asm
	MOV R0, #0x200003D8			; Test Status Address
	MOV R1, #0x0						
	STR R1, [R0]
	
	BLX test_general_case
	BLX tear_down
	BLX test_all_zero_inputs
	BLX tear_down
	BLX test_all_negative_inputs
	BLX tear_down
	BLX test_single_input
	BLX tear_down
	BLX test_largest_inputs
	BLX tear_down
	BLX test_empty_input

tear_down


test_general_case
	MOV R0, #0x200003D9			; Input array base address
	MOV S1, #-1
	STR S1, [R0]
	
	ADD R0, R0, 0x04				
	MOV S1, #1.3
	STR S1, [R0]
	
	ADD R0, R0, 0x04				
	MOV S1, #401
	STR S1, [R0]
	
	ADD R0, R0, 0x04				
	MOV S1, #10042.45
	STR S1, [R0]

	MOV R1 #0x2000042F			; Output array base address
	
	MOV R2, #0x200003B8			; Kalman state base address
	MOV S1, #1.3					
	STR S1, [R2]						; q
	MOV S1, #8124.2
	STR S1, [R0, 0x04]			; r
	MOV S1, #125.1
	STR S1, [R0, 0x08]			; x
	MOV S1, #-19083.2
	STR S1, [R0, 0x0C]			; p
	MOV S1, #1.32801
	STR S1, [R0, 0x10]			; q
	
	MOV R3, #4
	BLX
	BX LR
	
	; Do Check ;
check_values
		BX LR
	
	
	
test_all_zero_inputs




test_all_negative_inputs




test_single_input




test_largest_inputs




test_empty_input



	END
