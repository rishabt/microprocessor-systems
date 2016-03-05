/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program subroutine
	* Author						 : Ashraf Suyyagh
	* Version            : 1.0.0
	* Date							 : January 14th, 2016
  ******************************************************************************
  */
	
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "supporting_functions.h"
#include "lis3dsh.h"
#include "stm32f4xx_hal_gpio.h"

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config	(void);
void LIS3DSH_Config(void);

int main(void)
{	
  /* MCU Configuration----------------------------------------------------------*/

  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();
	
  /* Initialize all configured peripherals */
	LIS3DSH_Config();
	
	while (1){
	}
}

/** Accelerometer (LIS3DSH) Configuration*/
void LIS3DSH_Config(void){
	
	LIS3DSH_InitTypeDef LIS3DSH_InitTypeDef_Struct;
	LIS3DSH_DRYInterruptConfigTypeDef LIS3DSH_DRYInterruptConfigTypeDef_Struct;
	GPIO_InitTypeDef GPIO_Struct;
	
	/* Initialize accelerometer */
	LIS3DSH_InitTypeDef_Struct.Power_Mode_Output_DataRate = LIS3DSH_DATARATE_25;								// ODR2 => 25Hz
	LIS3DSH_InitTypeDef_Struct.Axes_Enable = LIS3DSH_XYZ_ENABLE;																	// enable x (roll) axis only
	LIS3DSH_InitTypeDef_Struct.Continous_Update = LIS3DSH_ContinousUpdate_Disabled;							// Only update data output registers if MSB and LSB are read
	LIS3DSH_InitTypeDef_Struct.AA_Filter_BW = LIS3DSH_AA_BW_50;																// Set anti-aliasing filter bandwidth to 50 Hz, since our sampling frequency is only 25Hz
	LIS3DSH_InitTypeDef_Struct.Full_Scale = LIS3DSH_FULLSCALE_2;																// Set accelerometer range to be plus or minus 2g
	LIS3DSH_InitTypeDef_Struct.Self_Test = LIS3DSH_SELFTEST_NORMAL;														// Enable self tests
	
	
	/* Setup accelerometer to generate interrupts when inputs are ready */
	LIS3DSH_DRYInterruptConfigTypeDef_Struct.Dataready_Interrupt = LIS3DSH_DATA_READY_INTERRUPT_ENABLED;														// Dataready Interrupt enable and routed to INT1 line
	LIS3DSH_DRYInterruptConfigTypeDef_Struct.Interrupt_signal = LIS3DSH_ACTIVE_HIGH_INTERRUPT_SIGNAL;															// Interrupt is active high
	LIS3DSH_DRYInterruptConfigTypeDef_Struct.Interrupt_type = LIS3DSH_INTERRUPT_REQUEST_PULSED;																// Interrupt pulsed
	
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

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  /* Prevent unused argument(s) compilation warning */
  __IO uint32_t tmpreg = 0x00;
  UNUSED(tmpreg); 
	
	if (GPIO_Pin == GPIO_PIN_0){
		float reading;
		LIS3DSH_ReadACC(&reading);
		printf("Printing: %f\n", reading);
	}
}


/** System Clock Configuration*/
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
	
	/*Configures SysTick to provide 1ms interval interrupts. SysTick is already 
	  configured inside HAL_Init, I don't kow why the CubeMX generates this call again*/
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

	/* This function sets the source clock for the internal SysTick Timer to be the maximum,
	   in our case, HCLK is now 168MHz*/
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

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

