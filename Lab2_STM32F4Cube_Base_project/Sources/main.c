/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : A program which showcases ADC and TIM3 under the new firmware
	                       The ADC
	* Author						 : Ashraf Suyyagh
	* Version            : 1.0.0
	* Date							 : January 14th, 2016
  ******************************************************************************
  */
	
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "supporting_functions.h"
#include "config.h"
#include "seven_segment.h"
#include "led_interface.h"
#include "utils.h"
#include "Kalmanfilter.h"

//extern

extern ADC_HandleTypeDef ADC1_Handle;

extern int INTERRUPT_RECEIVED;
extern int UPDATE_TEMP_FLAG;
extern int DISPLAY_FLAG;
float temperature_reading;
float temp_temperature_reading;
int ALARM_COUNTER;

kalman_state ks;

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config	(void);
void ADC_Config (void);
void read_temperature(void);

int main(void)
{
	ks.k = 0.0;
	ks.p = 1500.0;
	ks.q = 0.0000001;
	ks.r = 3200;
	ks.x = 29.4;

  /* MCU Configuration----------------------------------------------------------*/
  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();
	
  /* Configure the system clock */
  SystemClock_Config();
	
	/* Configure the ADC and GPIO */
	config_all();
	
	ALARM_COUNTER = 0;
	temp_temperature_reading = temperature_reading;
	
  while (1)
	{
		int one, two, three;
				
		if (INTERRUPT_RECEIVED)
		{
			
			read_temperature();
			INTERRUPT_RECEIVED = 0;
			if (UPDATE_TEMP_FLAG == 500)
			{
					temp_temperature_reading = temperature_reading;
					UPDATE_TEMP_FLAG = 0;
			}
			if (DISPLAY_FLAG == 1)
			{
				clear_all_segments();
				deactivate_digit(1);
				three = get_digit_in_place(temp_temperature_reading, 1);		
				activate_digit(3);
				show_seven_segment(three);
			} else if (DISPLAY_FLAG == 5)
			{
				clear_all_segments();
				deactivate_digit(3);
				two = get_digit_in_place(temp_temperature_reading, 10);		
				activate_digit(2);
				show_seven_segment(two);
				activate_decimal();
			} else if (DISPLAY_FLAG == 10)
			{
				clear_all_segments();
				deactivate_digit(2);
				one = get_digit_in_place(temp_temperature_reading, 100);		
				activate_digit(1);
				show_seven_segment(one);
				DISPLAY_FLAG = -5;

			}
			if (temp_temperature_reading >= 40)
			{
				ALARM_COUNTER += 1;
				raise_alarm();
				
				if(ALARM_COUNTER == 2000)
				{
					ALARM_COUNTER = 0;
				}
			}
			
		}
	}
	
	
	

}

void read_temperature(void)
{
	float temp = HAL_ADC_GetValue(&ADC1_Handle);
	
	temperature_reading  = temp*(3.3f/ 4096.0f);			// ADC 3.3 Volts per 2^12 steps (12 bit resolution in configuration)
	temperature_reading -= (float)0.76;															// reference of 25C at 760mV
	temperature_reading /= (float)0.025;															// slope of 25mV/1C
	temperature_reading += 25;																				// add the reference offset back
	
	temperature_reading = Kalmanfilter(temperature_reading, &ks);
	printf("%f\n", temperature_reading);
}


/** System Clock Configuration */
void SystemClock_Config(void){

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

  __PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  RCC_OscInitStruct.OscillatorType 	= RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState 			 	= RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState 		= RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource 	= RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM 				= 8;
  RCC_OscInitStruct.PLL.PLLN 				= 336;
  RCC_OscInitStruct.PLL.PLLP 				= RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ 				= 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK){Error_Handler(RCC_CONFIG_FAIL);};

  RCC_ClkInitStruct.ClockType 			= RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource 		= RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider 	= RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider 	= RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider 	= RCC_HCLK_DIV2;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5)!= HAL_OK){Error_Handler(RCC_CONFIG_FAIL);};
	
	/*Configures SysTick to provide 1ms interval interrupts.*/
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

	/* This function sets the source clock for the internal SysTick Timer to be the maximum,
	   in our case, HCLK is now 168MHz*/
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/**
   * @brief A function used to setup the ADC to sample Channel 16
   * @retval None
   */

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line){
}
#endif
