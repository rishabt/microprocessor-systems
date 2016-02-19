/**
 * @file utils.c
 * @brief utilities for lab 2
 **/
#include <stdio.h>
#include <math.h>
#include "utils.h"

int get_digit_in_place(float number, int place)
{
	int digit = (int) (number * 10);															// Multiplies number by 10
	digit = (int) (floor(digit/place)) % 10;											// Get digit corresponding to input place
	return digit;
}
