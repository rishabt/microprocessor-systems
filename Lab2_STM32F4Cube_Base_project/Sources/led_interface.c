/**
 * @file led_interface.c
 * @brief Interface for LED. Functions in this file 
 * 				are used for switching on/off LEDs and 
 *				raising alarm in high temperature conditions
 **/
#include <stdio.h>
#include "stm32f4xx.h"
#include "stm32f4xx_hal_gpio.h"
#include "led_interface.h"

// Global variable to manage alarm LEDs
extern int ALARM_COUNTER;

void light_green(void)
{
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET);						// Enable pin 12 of port D
}
void disable_green(void)
{
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET);					// Disable pin 12 of port D
}

void light_orange(void)
{
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET);						// Enable pin 13 of port D
}

void disable_orange(void)
{
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_RESET);					// Disable pin 13 of port D
}

void light_red(void)
{
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET);						// Enable pin 14 of port D
}

void disable_red(void)
{
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_RESET);					// Disable pin 14 of port D
}

void light_blue(void)
{
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_SET);						// Enable pin 15 of port D
}

void disable_blue(void)
{
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_RESET);					// Disable pin 15 of port D
}

void raise_alarm(void)
{
	/* 
	 * Conditions to keep track of LEDs so that they can be switched on
	 * in order and manage speed of rotation so that it is visible to
	 * eye.
	 */
	if(ALARM_COUNTER == 500) 
	{
		disable_blue();
		light_green();
	}
	if(ALARM_COUNTER == 1000)
	{
		disable_green();
		light_orange();
	}
	if(ALARM_COUNTER == 1500)
	{
		disable_orange();
		light_red();
	}
	if(ALARM_COUNTER == 2000)
	{
		disable_red();
		light_blue();
	}
}

void switch_off_alarm(void)
{
	disable_green();
	disable_orange();
	disable_red();
	disable_blue();
}
