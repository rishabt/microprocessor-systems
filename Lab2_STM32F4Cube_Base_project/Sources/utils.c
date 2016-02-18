#include <stdio.h>
#include <math.h>
#include "utils.h"

int get_digit_in_place(float number, int place)
{
	int digit = (int) (number * 10);
	
	digit = (int) (floor(digit/place)) % 10;
	
	return digit;
}
