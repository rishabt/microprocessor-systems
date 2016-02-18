#include <stdio.h>
#include "stm32f4xx.h"
#include "stm32f4xx_hal_gpio.h"
#include "led_interface.h"

extern int ALARM_COUNTER;

void light_green(void)
{
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET);
}
void disable_green(void)
{
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET);
}

void light_orange(void)
{
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET);
}

void disable_orange(void)
{
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_RESET);
}

void light_red(void)
{
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET);
}

void disable_red(void)
{
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_RESET);
}

void light_blue(void)
{
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_SET);
}

void disable_blue(void)
{
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_RESET);
}

void raise_alarm(void)
{
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
