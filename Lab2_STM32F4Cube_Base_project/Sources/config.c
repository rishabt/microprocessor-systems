/**
 * @file config.c
 * @brief Configurations for LED, 7 segment and ADC
 **/

#include <stdio.h>
#include "supporting_functions.h"
#include "stm32f4xx.h"
#include "stm32f4xx_hal_gpio.h"
#include "config.h"

ADC_HandleTypeDef ADC1_Handle;
GPIO_InitTypeDef GPIO_Struct;

void GPIO_Config(void)
{
	__HAL_RCC_GPIOE_CLK_ENABLE();																																						// Enable clock for port E
	GPIO_Struct.Pin = GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9
										| GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;	// Initialize the pins to be used
	GPIO_Struct.Mode = GPIO_MODE_OUTPUT_PP;																																	// Set mode to push and pull
	GPIO_Struct.Pull = GPIO_PULLUP;
	GPIO_Struct.Speed = GPIO_SPEED_HIGH;
	HAL_GPIO_Init(GPIOE, &GPIO_Struct);
}

void config_all(void)
{
	GPIO_Config();
}
