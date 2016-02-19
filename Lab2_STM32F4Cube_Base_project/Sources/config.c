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

void LED_Config(void)
{
	__HAL_RCC_GPIOD_CLK_ENABLE();																																						// Enable clock for port D
	GPIO_Struct.Pin = GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;																// Initialize pins corresponding to LEDs
	GPIO_Struct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_Struct.Pull = GPIO_PULLUP;
	GPIO_Struct.Speed = GPIO_SPEED_HIGH;
	HAL_GPIO_Init(GPIOD, &GPIO_Struct);
}


/** ADC Configuration */

void ADC_Config(void)
{	
	ADC_ChannelConfTypeDef ADC1_ChannelStruct;

	ADC1_Handle.Instance = ADC1;
	ADC1_Handle.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
	ADC1_Handle.Init.Resolution = ADC_RESOLUTION_12B;
	ADC1_Handle.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	ADC1_Handle.Init.ScanConvMode = DISABLE;
	ADC1_Handle.Init.EOCSelection = DISABLE;
	ADC1_Handle.Init.ContinuousConvMode = ENABLE;
	ADC1_Handle.Init.DMAContinuousRequests = DISABLE;
	ADC1_Handle.Init.NbrOfConversion = 1;
	ADC1_Handle.Init.DiscontinuousConvMode = DISABLE;
	ADC1_Handle.Init.NbrOfDiscConversion = 0;
	ADC1_Handle.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T1_CC1;
	ADC1_Handle.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;

	ADC1_ChannelStruct.Channel = ADC_CHANNEL_16;
	ADC1_ChannelStruct.Rank = 1;
	ADC1_ChannelStruct.SamplingTime = ADC_SAMPLETIME_480CYCLES;
	ADC1_ChannelStruct.Offset = 0;
	
	__ADC1_CLK_ENABLE();

	if(HAL_ADC_Init(&ADC1_Handle)!=HAL_OK){
		Error_Handler(ADC_INIT_FAIL);
	}
	HAL_ADC_ConfigChannel(&ADC1_Handle, &ADC1_ChannelStruct);		// configure channel with channel struct
	HAL_ADC_Start(&ADC1_Handle);																// set ADC to start polling mode

}

void config_all(void)
{
	GPIO_Config();
	LED_Config();
	ADC_Config();
}