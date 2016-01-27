	AREA myCode, CODE, READONLY
	IMPORT myString1
	IMPORT myString2
	EXPORT example4
example4
	LDR R0, =myString3
	LDR R1, [R0]
	LDR R2, =myString1
	STR R1, [R2]
	BX LR
		
	ALIGN
myString3	DCB	1,2,3,4
	END