	AREA myCode, CODE, READONLY
	IMPORT myString1
	IMPORT myString2
	EXPORT example4
example4
	LDR R0, =myString1
	LDR R1, [R0]
	MOV R2, #8
	STR R2, [R0]
	BX LR
		
	ALIGN
myString3	DCFS	1.1,2.2,3.3,4.4,5.5
	END