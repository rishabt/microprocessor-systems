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
#include "math.h"
#include "kalman_filter.h"

/* Private variables ---------------------------------------------------------*/
kalman_state ks_x;
kalman_state ks_y;
kalman_state ks_z;
float misalignment_and_offset_matrix[4][3] = {
	{0.9932,-0.0189,0.0662},
	{0.0170, 0.9770,0.0493},
	{-0.0026,-0.01717,0.9498},
	{0.0023, -0.0051,-0.0495}
};

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config	(void);
void LIS3DSH_Config(void);
void KalmanState_Config(void);

int main(void)
{	
  /* MCU Configuration----------------------------------------------------------*/

  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();
	
	/* Initialize kalman state */
	KalmanState_Config();
	
  /* Configure accelerometer */
	LIS3DSH_Config();
	
	while (1){
	}
}

void KalmanState_Config(void){
	ks_x.k = 0.0;
	ks_x.p = 1500.0;
	ks_x.q = 8;
	ks_x.r = 100;
	ks_x.x = 29.4;

	ks_y.k = 0.0;
	ks_y.p = 1500.0;
	ks_y.q = 8;
	ks_y.r = 100;
	ks_y.x = 29.4;
	
	ks_z.k = 0.0;
	ks_z.p = 1500.0;
	ks_z.q = 8;
	ks_z.r = 100;
	ks_z.x = 29.4;
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

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  /* Prevent unused argument(s) compilation warning */
  __IO uint32_t tmpreg = 0x00;
  UNUSED(tmpreg); 
	
	if (GPIO_Pin == GPIO_PIN_0){
		float readings[3];		// readings[0]->Ax, readings[1]->Ay, readings[2]->Az
		float pitch,x,y,z;
		float raw_matrix[4] = {0};
		float callibrated_matrix[3] = {0};
		int i,j;
		
		LIS3DSH_ReadACC(readings);
		
		raw_matrix[0] = readings[0];
		raw_matrix[1] = readings[1];
		raw_matrix[2] = readings[2];
		raw_matrix[3] = 1;

		for (i=0; i<3; i++){
			for (j=0; j<4; j++){
				callibrated_matrix[i] += raw_matrix[j]*misalignment_and_offset_matrix[j][i];
			}
		}
		
		x = Kalmanfilter(callibrated_matrix[0],&ks_x);
		y = Kalmanfilter(callibrated_matrix[1],&ks_y);
		z = Kalmanfilter(callibrated_matrix[2],&ks_z);
		
		pitch = atan2(x, sqrt(y*y + z*z)) * 180/ 3.14159265;
		printf("Pitch: %f\n", pitch);
		
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

