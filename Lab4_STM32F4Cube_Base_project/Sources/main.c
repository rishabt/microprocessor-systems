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


/** 
 * Display configuration parameters
 * used to set display state.
*/
int DISPLAY_ACC = 1;			// Accelerometer display 1:on/0:off
int DISPLAY_TEMP = 0; 		// Temperature display 1:on/0:off
int ACC_PITCH = 1;				// Pitch display 1:on/0:off
int ACC_ROLL = 0;					// Roll display 1:on/0:off
int toggle_on = 0;				// Toggle (counter) for flashing alarm (display on / display off)
extern int RAISE_ALARM;		// Raise alarm 1:on/0:off
int digit = 4;						// 7-Segment parameter used to select one of the 4 7-segs
int display_flag;					// counter incremented in timer and accessed/reset by threads to update the temporary reading at a slower frequency than acquisition

/**
 * Acceleration and temperature reading
 * configuration and data parameters.
*/
double tmp_temperature;							// temperature to be displayed
extern double tmp_pitch, tmp_roll;	// pitch and roll to be displayed
extern ADC_HandleTypeDef ADC1_Handle;
float temp;													// instantaneous ADC polled temperature (from interrupt callback)
float readings[3];									// instantaneous accelerometer readings (from interrupt callback)

/**
 * Configure semaphores to synchronize interrupts
 * with thread mode execution, and configure
 * mutex's to protect resources modified
 * across multiple threads.
*/
osMutexId kalman_filter_mutex;
osMutexDef (kalman_filter_mutex);

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


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
  /* Prevent unused argument(s) compilation warning */
  __IO uint32_t tmpreg = 0x00;
  UNUSED(tmpreg); 

	if (GPIO_Pin == GPIO_PIN_0) { // Accelerometer external interrupt through NVIC
		LIS3DSH_ReadACC(readings);
		HAL_NVIC_ClearPendingIRQ(EXTI0_IRQn);
		osSemaphoreRelease(accelerometer_select);
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim) {
	
	if (htim->Instance == TIM4){ // scan keypad
			osSemaphoreRelease(keypad_select);
	}	
	else if(htim->Instance == TIM2) { // poll adc configured to read temperature sensor
		osSemaphoreRelease(temperature_select);
	}	
	else if(htim->Instance == TIM3){ // update the display
		
		/*
		 * Increment counter used to update the display 
		 * value of temperature/pitch/roll at a
		 * slower frequency than acquisition.
		*/
		display_flag++;								

		
		if (((toggle_on < 10000) && RAISE_ALARM == 1) || (RAISE_ALARM == 0)) { // Enable display
			if(DISPLAY_ACC == 1){ // Show accelerometer reading
					digit = digit - 1;
					if(digit == 0)
						digit = 4;
					
					if (ACC_PITCH==1) 
						display(tmp_pitch);			
					else if (ACC_ROLL==1) 
						display(tmp_roll);			
			}
			else if(DISPLAY_TEMP == 1) { // Show temperature value
					digit = digit - 1;
					if(digit == 0)
						digit = 4;
					display(tmp_temperature);			
			}
			toggle_on++;
		} else if ((toggle_on >= 10000) && RAISE_ALARM == 1){ // Disable the display
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

	
	/**************** Accelerometer thread setup *********************************/
	acc_kalmanState_Config();
	
	accelerometer_select = osSemaphoreCreate(osSemaphore(accelerometer_select), 1);
	accelerometer_set_semaphore(accelerometer_select);
	
	/************** Temperature Sensor Thread Setup *****************************/
	temp_kalmanState_Config();
	
	temperature_select = osSemaphoreCreate(osSemaphore(temperature_select), 1);
	temperature_set_semaphore(temperature_select);
	
	/************** Keyboard Thread Setup ***************************************/
	
	keypad_select = osSemaphoreCreate(osSemaphore(keypad_select), 1);
	keypad_set_semaphore(keypad_select);

	/************** Mutex's for Shared Resources *****************************/
	
	display_flag_mutex = osMutexCreate(osMutex(display_flag_mutex));
	kalman_filter_mutex = osMutexCreate(osMutex(kalman_filter_mutex));


	start_Thread_Keypad();																	 /* Create keypad thread 							*/
	start_Thread_Accelerometer();                       		 /* Create accelerometer thread       */
	start_Thread_Temperature_Sensor();                       /* Create Temperature sensor thread  */
	
	/* User codes ends here*/
  
	osKernelStart();                          /* start thread execution         */
}
