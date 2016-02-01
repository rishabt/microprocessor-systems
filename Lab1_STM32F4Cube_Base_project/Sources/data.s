	AREA myData, DATA, READWRITE
		
input_array_base_address		DCFS	0.0,0.0,0.0,0.0
output_array_base_address 	DCFS	0.0,0.0,0.0,0.0
kalman_state_base_address 	DCFS	0.0,0.0,0.0,0.0
test_status_base_address 		DCD		0,0,0,0,0,0							; Keep status of each test here (1-success, 0-fail)


	EXPORT input_array_base_address
	EXPORT output_array_base_address
	EXPORT kalman_state_base_address
	EXPORT test_status_base_address
	END