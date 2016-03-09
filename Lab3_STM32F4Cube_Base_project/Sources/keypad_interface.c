#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"
#include "keypad_interface.h"

/*
 * Pin 1 -> PB0
 * Pin 2 -> PB1
 * Pin 3 -> PB2
 * Pin 4 -> PB4
 * Pin 5 -> PB5
 * Pin 6 -> PB7
 * Pin 7 -> PB8
 * Pin 8 -> PB11
 *
 * 
 * rows = {PB1, PB2, PB4, PB5}
 * colums = {PB11, PB8, PB7}
 */
 
GPIO_InitTypeDef Rows_Struct;
GPIO_InitTypeDef Columns_Struct;
 
void rows_init(void)
{
	//__HAL_RCC_SYSCFG_CLK_ENABLE();
	
	//__HAL_RCC_ENABLE_IT(RCC_IT_PLLRDY);
	
	/* Initialize the rows */
	__HAL_RCC_GPIOB_CLK_ENABLE();																																						// Enable clock for port E
	Rows_Struct.Pin = GPIO_PIN_13 | GPIO_PIN_12 | GPIO_PIN_4 | GPIO_PIN_5;																		// Initialize the pins to be used
	Rows_Struct.Mode = GPIO_MODE_IT_FALLING;																																			// Set mode to push and pull
	Rows_Struct.Pull = GPIO_PULLUP;
	Rows_Struct.Speed = GPIO_SPEED_LOW;
	HAL_GPIO_Init(GPIOB, &Rows_Struct);
	
	/* Initialize the columns */
	//__HAL_RCC_GPIOB_CLK_ENABLE();																																					// Enable clock for port E
	Columns_Struct.Pin = GPIO_PIN_11 | GPIO_PIN_8 | GPIO_PIN_7;																							// Initialize the pins to be used
	Columns_Struct.Mode = GPIO_MODE_OUTPUT_PP;																															// Set mode to push and pull
	Columns_Struct.Pull = GPIO_NOPULL;
	Columns_Struct.Speed = GPIO_SPEED_LOW;
	HAL_GPIO_Init(GPIOB, &Columns_Struct);
	
	/*EXTI->IMR |= (1<<1);
	EXTI->FTSR |= (1<<1);
	
	SYSCFG->EXTICR[0] &= SYSCFG_EXTICR1_EXTI0_PA;*/

  //EXTI-> = EXTI_Mode_Interrupt;
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
	HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0,0);
	HAL_NVIC_ClearPendingIRQ(EXTI15_10_IRQn);
	
	HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
	HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0,0);
	HAL_NVIC_ClearPendingIRQ(EXTI9_5_IRQn);
	
	HAL_NVIC_EnableIRQ(EXTI4_IRQn);
	HAL_NVIC_SetPriority(EXTI4_IRQn, 0,0);
	HAL_NVIC_ClearPendingIRQ(EXTI4_IRQn);
	
	
}

void init_columns(void)
{
	//__HAL_RCC_SYSCFG_CLK_ENABLE();
	
	/* Initialize the columns */
	//__HAL_RCC_GPIOB_CLK_ENABLE();																																					// Enable clock for port E
	Columns_Struct.Pin = GPIO_PIN_11 | GPIO_PIN_8 | GPIO_PIN_7;																							// Initialize the pins to be used
	Columns_Struct.Mode = GPIO_MODE_INPUT;																															// Set mode to push and pull
	Columns_Struct.Pull = GPIO_PULLUP;
	Columns_Struct.Speed = GPIO_SPEED_LOW;
	HAL_GPIO_Init(GPIOB, &Columns_Struct);
	
		/* Initialize the rows */
	__HAL_RCC_GPIOB_CLK_ENABLE();																																						// Enable clock for port E
	Rows_Struct.Pin = GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_4 | GPIO_PIN_5;																		// Initialize the pins to be used
	Rows_Struct.Mode = GPIO_MODE_OUTPUT_PP;																																			// Set mode to push and pull
	Rows_Struct.Pull = GPIO_PULLUP;
	Rows_Struct.Speed = GPIO_SPEED_LOW;
	HAL_GPIO_Init(GPIOB, &Rows_Struct);
	
}

void get_input(void)
{
	
}

/*void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin == 1)
	{
		
	}
}*/
