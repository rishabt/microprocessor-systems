/**
 * @file utils.c
 * @brief utilities for lab 2
 **/
#include <stdio.h>
#include <math.h>
#include "utils.h"
#include "seven_segment.h"

extern int digit;

int get_digit_in_place(float number, int place)
{
	int digit = (int) (number * 10);															// Multiplies number by 10
	digit = (int) (floor(digit/place)) % 10;											// Get digit corresponding to input place
	return digit;
}

void display(float number)
{
	int one, two, three;
	
	if(number < 10){																													// Two decimal points
		
		number = number * 10;
		one = get_digit_in_place(number, 1);
		two = get_digit_in_place(number, 10);
		three = get_digit_in_place(number, 100);
		
		if(digit == 1){
			clear_all_segments();
			deactivate_degree();
			deactivate_digit(2);
			
			activate_digit(1);
			activate_decimal();
			show_seven_segment(three);
		}
		else if(digit == 2){
			clear_all_segments();
			deactivate_degree();
			deactivate_digit(3);
			
			activate_digit(2);
			show_seven_segment(two);
		}
		else if(digit == 3){
			clear_all_segments();
			deactivate_degree();
			deactivate_digit(1);
			
			activate_digit(3);
			show_seven_segment(one);
		}
		else if(digit == 4){		
			activate_degree();
		}
		
	}
	else if(number > 10 && number < 100){																			// One decimal point
		
		number = number;
		one = get_digit_in_place(number, 1);
		two = get_digit_in_place(number, 10);
		three = get_digit_in_place(number, 100);
		
		if(digit == 1){
			clear_all_segments();
			deactivate_degree();
			deactivate_digit(2);
			
			activate_digit(1);
			show_seven_segment(three);
		}
		else if(digit == 2){
			clear_all_segments();
			deactivate_degree();
			deactivate_digit(3);
			
			activate_digit(2);
			activate_decimal();
			show_seven_segment(two);
		}
		else if(digit == 3){
			clear_all_segments();
			deactivate_degree();
			deactivate_digit(1);
			
			activate_digit(3);
			show_seven_segment(one);
		}	
		else if(digit == 4){			
			activate_degree();
		}
		
	}
	else if(number > 100){																										// No decimal point
		
		number = number/10;
		one = get_digit_in_place(number, 1);
		two = get_digit_in_place(number, 10);
		three = get_digit_in_place(number, 100);
		
		if(digit == 1){
			clear_all_segments();
			deactivate_degree();
			deactivate_digit(2);
			
			activate_digit(1);
			show_seven_segment(three);
		}
		else if(digit == 2){
			clear_all_segments();
			deactivate_degree();
			deactivate_digit(3);
			
			activate_digit(2);
			show_seven_segment(two);
		}
		else if(digit == 3){
			clear_all_segments();
			deactivate_degree();
			deactivate_digit(1);
			
			activate_digit(3);
			show_seven_segment(one);
		}
		else if(digit == 4){
			activate_degree();
		}
	}
				
}
