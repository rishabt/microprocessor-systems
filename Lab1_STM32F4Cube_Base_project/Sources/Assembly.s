	AREA myCode, CODE, READONLY
	EXPORT example3
example3
	MOV R1, #25
	MOV R2, #75
	ADD R1, R2, R1
	BX LR
	END