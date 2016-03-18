/**
 * @file config.c
 * @brief Configurations for LED, 7 segment and ADC
 **/

#include <stdio.h>
#include "supporting_functions.h"
#include "lis3dsh.h"
#include "stm32f4xx.h"
#include "stm32f4xx_hal_gpio.h"
#include "config.h"
#include "stm32f4xx_hal_tim.h"
#include "stm32f4xx_hal_rcc.h"
#include "cmsis_os.h"     

ADC_HandleTypeDef ADC1_Handle;
GPIO_InitTypeDef GPIO_Struct;
GPIO_InitTypeDef Degree_Struct;
TIM_HandleTypeDef TIM2_Handle;
TIM_HandleTypeDef TIM3_Handle;

void GPIO_Config(void)
{
	__HAL_RCC_GPIOE_CLK_ENABLE();																																						// Enable clock for port E
	GPIO_Struct.Pin = GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9
										| GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;	// Initialize the pins to be used
	GPIO_Struct.Mode = GPIO_MODE_OUTPUT_PP;																																	// Set mode to push and pull
	GPIO_Struct.Pull = GPIO_PULLUP;
	GPIO_Struct.Speed = GPIO_SPEED_HIGH;
	HAL_GPIO_Init(GPIOE, &GPIO_Struct);
	
	__HAL_RCC_GPIOC_CLK_ENABLE();
	Degree_Struct.Pin = GPIO_PIN_11;
	Degree_Struct.Mode = GPIO_MODE_OUTPUT_PP;																																	// Set mode to push and pull
	Degree_Struct.Pull = GPIO_PULLUP;
	Degree_Struct.Speed = GPIO_SPEED_HIGH;
	HAL_GPIO_Init(GPIOC, &Degree_Struct);
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
	HAL_ADC_Start(&ADC1_Handle);	

}


/** Accelerometer (LIS3DSH) Configuration*/
void LIS3DSH_Config(void){
	
	LIS3DSH_InitTypeDef LIS3DSH_InitTypeDef_Struct;
	LIS3DSH_DRYInterruptConfigTypeDef LIS3DSH_DRYInterruptConfigTypeDef_Struct;
	GPIO_InitTypeDef GPIO_Struct;
	
	/* Initialize accelerometer */
	LIS3DSH_InitTypeDef_Struct.Power_Mode_Output_DataRate = LIS3DSH_DATARATE_25;								// ODR2 => 25Hz
	LIS3DSH_InitTypeDef_Struct.Axes_Enable = LIS3DSH_XYZ_ENABLE;																// enable X,Y,Z
	LIS3DSH_InitTypeDef_Struct.Continous_Update = LIS3DSH_ContinousUpdate_Disabled;							// Only update data output registers if MSB and LSB are read
	LIS3DSH_InitTypeDef_Struct.AA_Filter_BW = LIS3DSH_AA_BW_50;																	// Set anti-aliasing filter bandwidth to 50 Hz, since our sampling frequency is only 25Hz
	LIS3DSH_InitTypeDef_Struct.Full_Scale = LIS3DSH_FULLSCALE_2;																// Set accelerometer range to be plus or minus 2g
	LIS3DSH_InitTypeDef_Struct.Self_Test = LIS3DSH_SELFTEST_NORMAL;															// Enable self tests
	
	
	/* Setup accelerometer to generate interrupts when inputs are ready */
	LIS3DSH_DRYInterruptConfigTypeDef_Struct.Dataready_Interrupt = LIS3DSH_DATA_READY_INTERRUPT_ENABLED;	// Dataready Interrupt enable and routed to INT1 line
	LIS3DSH_DRYInterruptConfigTypeDef_Struct.Interrupt_signal = LIS3DSH_ACTIVE_HIGH_INTERRUPT_SIGNAL;			// Interrupt is active high
	LIS3DSH_DRYInterruptConfigTypeDef_Struct.Interrupt_type = LIS3DSH_INTERRUPT_REQUEST_PULSED;						// Interrupt pulsed
	
	/* Enable GPIO PINE 0 hard-wired to accelerometer's INT1 line */
	__HAL_RCC_GPIOE_CLK_ENABLE();																											// Enable clock for port E
	GPIO_Struct.Pin = GPIO_PIN_0;																											// Initialize the pins to be used
	GPIO_Struct.Mode = GPIO_MODE_IT_RISING;
	GPIO_Struct.Pull = GPIO_NOPULL;
	GPIO_Struct.Speed = GPIO_SPEED_MEDIUM;	
	HAL_GPIO_Init(GPIOE, &GPIO_Struct);
	
	//printf("* LIS3DSH_Config * \n");
	
	HAL_NVIC_EnableIRQ(EXTI0_IRQn);
	HAL_NVIC_SetPriority(EXTI0_IRQn, 0,6);
	HAL_NVIC_ClearPendingIRQ(EXTI0_IRQn);

	LIS3DSH_Init(&LIS3DSH_InitTypeDef_Struct);
	LIS3DSH_DataReadyInterruptConfig(&LIS3DSH_DRYInterruptConfigTypeDef_Struct);
}

void Tim3_Config(void)
{
	TIM_Base_InitTypeDef TIM_TimeBaseStructure;
	
	TIM_TimeBaseStructure.Period = 20;
	TIM_TimeBaseStructure.Prescaler = 100;
	TIM_TimeBaseStructure.ClockDivision = TIM_CLOCKDIVISION_DIV4;
	TIM_TimeBaseStructure.CounterMode = TIM_COUNTERMODE_UP;
	//TIM_TimeBaseStructure.RepetitionCounter = 0;
	
	/* Fill in the TIM handle with the required timer and init struct*/
	TIM3_Handle.Instance = TIM3;
	TIM3_Handle.Init = TIM_TimeBaseStructure;
	TIM3_Handle.Channel = HAL_TIM_ACTIVE_CHANNEL_CLEARED;
	TIM3_Handle.Lock = HAL_UNLOCKED;
	TIM3_Handle.State = HAL_TIM_STATE_READY;

	HAL_TIM_Base_MspInit(&TIM3_Handle);
	
	/* Enable clock for TIM3 */
	__TIM3_CLK_ENABLE();
	
	HAL_TIM_Base_Init(&TIM3_Handle);
	HAL_TIM_Base_Start_IT(&TIM3_Handle);
	
	//TIM_Base_SetConfig(TIM3, &TIM_TimeBaseStructure);
	
	/* Configure NVIC */
	HAL_NVIC_EnableIRQ(TIM3_IRQn);
	HAL_NVIC_SetPriority(TIM3_IRQn, 0,0);
	//HAL_NVIC_ClearPendingIRQ(TIM3_IRQn);
}

void Tim2_Config(void)
{
	TIM_Base_InitTypeDef TIM_TimeBaseStructure;
	
	TIM_TimeBaseStructure.Period = 20;
	TIM_TimeBaseStructure.Prescaler = 42000;
	TIM_TimeBaseStructure.ClockDivision = TIM_CLOCKDIVISION_DIV4;
	TIM_TimeBaseStructure.CounterMode = TIM_COUNTERMODE_UP;
	//TIM_TimeBaseStructure.RepetitionCounter = 0;
	
	/* Fill in the TIM handle with the required timer and init struct*/
	TIM2_Handle.Instance = TIM2;
	TIM2_Handle.Init = TIM_TimeBaseStructure;
	TIM2_Handle.Channel = HAL_TIM_ACTIVE_CHANNEL_CLEARED;
	TIM2_Handle.Lock = HAL_UNLOCKED;
	TIM2_Handle.State = HAL_TIM_STATE_READY;

	HAL_TIM_Base_MspInit(&TIM2_Handle);
	
	/* Enable clock for TIM3 */
	__TIM2_CLK_ENABLE();
	
	HAL_TIM_Base_Init(&TIM2_Handle);
	HAL_TIM_Base_Start_IT(&TIM2_Handle);
	
	//TIM_Base_SetConfig(TIM3, &TIM_TimeBaseStructure);
	
	/* Configure NVIC */
	HAL_NVIC_EnableIRQ(TIM2_IRQn);
	HAL_NVIC_SetPriority(TIM2_IRQn, 0,0);
	//HAL_NVIC_ClearPendingIRQ(TIM3_IRQn);
}

void config_all(void)
{
	GPIO_Config();
	ADC_Config();
	LIS3DSH_Config();
	Tim2_Config();
	Tim3_Config();
}
