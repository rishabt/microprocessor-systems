/**
  ******************************************************************************
  * @file    HAL/HAL_TimeBase/Src/stm32f4xx_it.c 
  * @author  MCD Application Team
  * @version   V1.2.4
  * @date      13-November-2015
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2015 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_it.h"
#include "lis3dsh.h"
#include "seven_segment.h"

extern TIM_HandleTypeDef TIM_Handle;
extern float pitch;
extern int target_angle;

int upper_bound;
int lower_bound;
int in_range = 0;
extern float tmp_pitch;

int display_flag = 0;
int digit = 3;

char up = 'U';
char down = 'D';

void display(float number);
/** @addtogroup STM32F4xx_HAL_Examples
  * @{
  */

/** @addtogroup GPIO_EXTI
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief   This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
	HAL_IncTick();
}

/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f4xx.s).                                               */
/******************************************************************************/
/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
void EXTI0_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
}

/**
  * @brief  This function handles EXTI 9 to 5 interrupt for keypad rows
  * @param  None
  * @retval None
  */
void EXTI9_5_IRQHandler(void)
{
	if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_5) == 0)																									// Row 1
	{
				HAL_Delay(300);

		HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_5);
	}
	else if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_6) == 0)																							// Row 2
	{
						HAL_Delay(300);

		HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_6);
	} 
	else if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_8) == 0) 																						// Row 3
	{	
						HAL_Delay(300);

		HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_8);
	}
	else if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_9) == 0)																						// Row 4
	{
						HAL_Delay(300);

		HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_9);
	}
}

/**
  * @brief  This function handles TIM3 interrupt
  * @param  None
  * @retval None
  */
void TIM3_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&TIM_Handle);
}

/**
  * @brief  This function handles callback for TIM3
  * @param  htim* Timer handler
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim)
{
	if(htim->Instance == TIM3)
	{
		display_flag++;
		
		upper_bound = target_angle + 5;
		lower_bound = target_angle - 5;
		
		
		if(pitch < lower_bound)
		{
			in_range = 0;
			
			clear_all_segments();
			deactivate_all_digits();
			deactivate_degree();
			activate_digit(3);
			show_arrow(up);
		}
		else if(pitch > upper_bound)
		{
			in_range = 0;
			
			clear_all_segments();
			deactivate_all_digits();
			deactivate_degree();
			activate_digit(3);
			show_arrow(down);
		}
		else
		{
			in_range = 1;
		}

		if(in_range == 1)
		{
			digit = digit - 1;
			
			if(digit == 0)
			{
				digit = 4;
			}
			
			display(tmp_pitch);
		}
		
	}
}

/**
  * @}
  */ 

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
