#include <stdio.h>
#include "stm32f4xx.h"
#include "stm32f4xx_hal_gpio.h"
#include "seven_segment.h"

void show_seven_segment(int number)
{
	switch(number)
	{
		case 0:
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_12, GPIO_PIN_SET);		// digit 1
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_5, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_6, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_SET);
			break;
		case 1:
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_5, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_6, GPIO_PIN_SET);
			break;
		case 2:
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_5, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7, GPIO_PIN_SET);
			break;
		case 3:
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_5, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_6, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7, GPIO_PIN_SET);
			break;
		case 4:
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_5, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_6, GPIO_PIN_SET);
			break;
		case 5:
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_6, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, GPIO_PIN_SET);
			break;
		case 6:
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_6, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, GPIO_PIN_SET);
			break;
		case 7:
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_5, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_6, GPIO_PIN_SET);
			break;
		case 8:
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_5, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_6, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, GPIO_PIN_SET);
			break;
		case 9:
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_5, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_6, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, GPIO_PIN_SET);
			break;
	}
}

void clear_all_segments(void)
{
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_5, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_6, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_11, GPIO_PIN_RESET);
}

void activate_digit(int number)
{
	switch(number)
	{
		case 1:
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_12, GPIO_PIN_SET);
			break;
		case 2:
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_13, GPIO_PIN_SET);
			break;
		case 3:
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_14, GPIO_PIN_SET);
			break;
		case 4:
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_15, GPIO_PIN_SET);
			break;
	}
}

void deactivate_digit(int number)
{
	switch(number)
	{
		case 1:
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_12, GPIO_PIN_RESET);
			break;
		case 2:
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_13, GPIO_PIN_RESET);
			break;
		case 3:
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_14, GPIO_PIN_RESET);
			break;
		case 4:
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_15, GPIO_PIN_RESET);
			break;
	}
}

void activate_decimal(void)
{
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_11, GPIO_PIN_SET);
}
