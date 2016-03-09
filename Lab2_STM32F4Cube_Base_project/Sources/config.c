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

ADC_HandleTypeDef ADC1_Handle;
GPIO_InitTypeDef GPIO_Struct;
GPIO_InitTypeDef Degree_Struct;
TIM_HandleTypeDef TIM_Handle;

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
	
	printf("* LIS3DSH_Config * \n");
	
	HAL_NVIC_EnableIRQ(EXTI0_IRQn);
	HAL_NVIC_SetPriority(EXTI0_IRQn, 0,1);
	HAL_NVIC_ClearPendingIRQ(EXTI0_IRQn);

	LIS3DSH_Init(&LIS3DSH_InitTypeDef_Struct);
	LIS3DSH_DataReadyInterruptConfig(&LIS3DSH_DRYInterruptConfigTypeDef_Struct);
}

void Timer_Config(void)
{
	TIM_Base_InitTypeDef TIM_TimeBaseStructure;
	
	TIM_TimeBaseStructure.Period = 200;																								// 20000 - 1 for 1 second  200
	TIM_TimeBaseStructure.Prescaler = 1000;																						// 42000 - 1 for 1 second  3000
	TIM_TimeBaseStructure.ClockDivision = TIM_CLOCKDIVISION_DIV4;
	TIM_TimeBaseStructure.CounterMode = TIM_COUNTERMODE_UP;
	//TIM_TimeBaseStructure.RepetitionCounter = 0;
	
	/* Fill in the TIM handle with the required timer and init struct*/
	TIM_Handle.Instance = TIM3;
	TIM_Handle.Init = TIM_TimeBaseStructure;
	TIM_Handle.Channel = HAL_TIM_ACTIVE_CHANNEL_CLEARED;
	TIM_Handle.Lock = HAL_UNLOCKED;
	TIM_Handle.State = HAL_TIM_STATE_READY;

	HAL_TIM_Base_MspInit(&TIM_Handle);
	
	/* Enable clock for TIM3 */
	__TIM3_CLK_ENABLE();
	
	HAL_TIM_Base_Init(&TIM_Handle);
	HAL_TIM_Base_Start_IT(&TIM_Handle);
	
	//TIM_Base_SetConfig(TIM3, &TIM_TimeBaseStructure);
	
	/* Configure NVIC */
	HAL_NVIC_EnableIRQ(TIM3_IRQn);
	HAL_NVIC_SetPriority(TIM3_IRQn, 9,9);
	//HAL_NVIC_ClearPendingIRQ(TIM3_IRQn);
}

void config_all(void)
{
	GPIO_Config();
	LIS3DSH_Config();
	Timer_Config();
}
