	AREA myData, DATA, READWRITE
	EXPORT myString1
	EXPORT myString2
myString1	DCB	1,2,3,4,5
myString2	FILL 50,0xAB
	END