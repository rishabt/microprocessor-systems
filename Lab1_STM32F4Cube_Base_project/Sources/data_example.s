	AREA myData, DATA, READWRITE
myString1	DCB		7,9,3,8
myString2	FILL 50,0xAB
	EXPORT myString1
	EXPORT myString2
	END