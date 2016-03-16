/**
 * @file utils.h
 * @brief Utilities for lab
 **/
#ifndef UTILS_H
#define UTILS_H

/**
 * @brief Gets digit in place of the number provided
 * 
 * Can be used to get the digit in place of given number. 
 * The places are shifter by 1 place. Eg. In 25.3
 * 3 -> place = 1
 * 5 -> place = 10
 * 2 -> place = 100
 * @param
 * @return
 **/
int get_digit_in_place(float number, int place);

void display(float number);

#endif
