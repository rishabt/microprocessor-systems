/**
 * @file seven_segment.h
 * @brief Functions to control the 7 segment display
 **/
#ifndef SEVEN_SEG_H
#define SEVEN_SEG_H

/**
 * @brief Displays number on seven segment display.
 *				The number will be displayed on the digit selected.
 * @param number Number to be displayed
 * @return Void
 **/
void show_seven_segment(int number);

/**
 * @brief Displays character on seven segment display.
 *				The number will be displayed on the digit selected.
 * @param character Character to be displayed
 * @return Void
 **/
void show_arrow(char c);

/**
 * @brief Clears the seven segment display.
 * @param
 * @return Void
 **/
void clear_all_segments(void);

/**
 * @brief Activates the inputted digit on the display
 * @param	number The digit number to be selected
 * @return Void
 **/
void activate_digit(int number);

/**
 * @brief Activates the decimal point
 * @param
 * @return Void
 **/
void activate_decimal(void);

void activate_degree(void);

void deactivate_degree(void);

/**
 * @brief Deactivate the digit
 * @param number Digit number
 * @return Void
 **/
void deactivate_digit(int number);

/**
 * @brief Deactivate all digits
 * @param Void
 * @return Void
 **/
void deactivate_all_digits(void);
	
#endif
