/*******************************************************************************
  * @file    main.c
  * @author  Ashraf Suyyagh
	* @version V1.2.0
  * @date    17-January-2016
  * @brief   This file demonstrates flasing one LED at an interval of one second
	*          RTX based using CMSIS-RTOS 
  ******************************************************************************
  */

#include "stm32f4xx_hal.h"              // Keil::Device:STM32Cube HAL:Common
#include "cmsis_os.h"                   // ARM::CMSIS:RTOS:Keil RTX
#include "RTE_Components.h"             // Component selection
#include "thread.h"
#include "config.h"
#include "math.h"
#include "supporting_functions.h"
#include "lis3dsh.h"
#include "accelerometer_interface.h"
#include "temperature_interface.h"
#include "keypad_interface.h"
#include "seven_segment.h"
#include "utils.h"

int DISPLAY_ACC = 1;
int DISPLAY_TEMP = 0;
extern int RAISE_ALARM;
int ACC_PITCH = 1;
int ACC_ROLL = 0;
int toggle_on = 0;

extern void initializeLED_IO			(void);
extern void start_Thread_LED			(void);
extern void Thread_LED(void const *argument);
extern osThreadId tid_Thread_LED;
double tmp_temperature;
extern double tmp_pitch, tmp_roll;
extern int DISPLAY_OPTION;
extern int key_number;
extern int pitch;

extern ADC_HandleTypeDef ADC1_Handle;
float temp;

int digit = 4;

float readings[3];

char down = 'D';
char up = 'U';

int display_flag, in_range;
double upper_bound, lower_bound;

osMutexId display_flag_mutex;
osMutexDef (display_flag_mutex);

osSemaphoreId keypad_select;
osSemaphoreDef(keypad_select);

osSemaphoreId accelerometer_select;
osSemaphoreDef(accelerometer_select);

osSemaphoreId temperature_select;
osSemaphoreDef(temperature_select);


/**
	These lines are mandatory to make CMSIS-RTOS RTX work with te new Cube HAL
*/
#ifdef RTE_CMSIS_RTOS_RTX
extern uint32_t os_time;

uint32_t HAL_GetTick(void) { 
  return os_time; 
}
#endif

/**
  * System Clock Configuration
  */
void SystemClock_Config(void) {
  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();

  /* The voltage scaling allows optimizing the power consumption when the
     device is clocked below the maximum system frequency (see datasheet). */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
     clocks dividers */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 |
                                RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  /* Prevent unused argument(s) compilation warning */
  __IO uint32_t tmpreg = 0x00;
  UNUSED(tmpreg); 

	if (GPIO_Pin == GPIO_PIN_0)
	{
		
		LIS3DSH_ReadACC(readings);
		
		HAL_NVIC_ClearPendingIRQ(EXTI0_IRQn);
		osSemaphoreRelease(accelerometer_select);
		
	}
	
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim)
{
	if (htim->Instance == TIM4)
	{
			osSemaphoreRelease(keypad_select);
	}
	else if(htim->Instance == TIM2)
	{
		temp = HAL_ADC_GetValue(&ADC1_Handle);
		
		osSemaphoreRelease(temperature_select);
	}	
	
	else if(htim->Instance == TIM3)
	{
		osMutexWait(display_flag_mutex, osWaitForever);
		display_flag++;
		osMutexRelease(display_flag_mutex);

		if (((toggle_on < 10000) && RAISE_ALARM == 1) || (RAISE_ALARM == 0))
		{
			if(DISPLAY_ACC == 1)
			{
					digit = digit - 1;
					if(digit == 0)
					{
						digit = 4;
					}
					if (ACC_PITCH==1) 
						display(tmp_pitch);			
					else if (ACC_ROLL==1) 
						display(tmp_roll);			
			}
			else if(DISPLAY_TEMP == 1)
			{
					digit = digit - 1;
					if(digit == 0)
					{
						digit = 4;
					}
					display(tmp_temperature);			
			}
			toggle_on++;
		} else if ((toggle_on >= 10000) && RAISE_ALARM == 1){
			clear_all_segments();
			toggle_on++;
			if (toggle_on > 20000)
				toggle_on = 0;
		}
	}
}

/**
  * Main function
  */
int main (void) {

  osKernelInitialize();                     /* initialize CMSIS-RTOS          */

  HAL_Init();                               /* Initialize the HAL Library     */

  SystemClock_Config();                     /* Configure the System Clock     */

	config_all();
		
	//initializeLED_IO();                       /* Initialize LED GPIO Buttons    */
  //start_Thread_LED();                       /* Create LED thread              */
	
	/**************** Accelerometer thread setup *********************************/
	acc_kalmanState_Config();
	
	accelerometer_select = osSemaphoreCreate(osSemaphore(accelerometer_select), 1);
	accelerometer_set_semaphore(accelerometer_select);
	
	/************** Temperature Sensor Thread Setup *****************************/
	temp_kalmanState_Config();
	
	temperature_select = osSemaphoreCreate(osSemaphore(temperature_select), 1);
	temperature_set_semaphore(temperature_select);
	
	/************** Keyboard Thread Setup *****************************/
	
	keypad_select = osSemaphoreCreate(osSemaphore(keypad_select), 1);
	keypad_set_semaphore(keypad_select);

	
	display_flag_mutex = osMutexCreate(osMutex(display_flag_mutex));

	start_Thread_Keypad();
	start_Thread_Accelerometer();                       /* Create accelerometer thread              */
	start_Thread_Temperature_Sensor();                       /* Create Temperature sensor thread              */
	
	/* User codes ends here*/
  
	osKernelStart();                          /* start thread execution         */
}
