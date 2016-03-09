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
#include "config.h"
#include "seven_segment.h"
#include "utils.h"
#include "keypad_interface.h"

extern TIM_HandleTypeDef TIM_Handle;
extern int display_flag;
//extern int digit;

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

int target_angle = 0;

int KEYPAD_DOWN_FLAG = 1;
int count;
float pitch;
float tmp_pitch;
float degree;

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config	(void);
void LIS3DSH_Config(void);
void KalmanState_Config(void);
void display(float number);

int main(void)
{	
  /* MCU Configuration----------------------------------------------------------*/

  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();
	
	/* Initialize kalman state */
	KalmanState_Config();
	
	rows_init();
	
	config_all();
	tmp_pitch = pitch;
	
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


int enter = 0;
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  /* Prevent unused argument(s) compilation warning */
  __IO uint32_t tmpreg = 0x00;
  UNUSED(tmpreg); 

	if (GPIO_Pin == GPIO_PIN_0){
		float readings[3];		// readings[0]->Ax, readings[1]->Ay, readings[2]->Az
		float x,y,z;
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
		
		if ( (y<0) && (z<0) && (x>0)){
			pitch = (90-pitch) + 90;
		} else if ( (y<0) && (z<0) && (x<0) ) {
			pitch = 180 + (-1*pitch);
		} else if ( (x<0) && (y>0) && (z>0)) {
			pitch = 360 + pitch;
		}
		
		if(display_flag % 5 == 0)
		{
			tmp_pitch = pitch;
		}
		
		
	}

	if ((target_angle < 100) && enter == 0)
	{
		if(GPIO_Pin == GPIO_PIN_5)
		{
			
			init_columns();
			
			if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_13) == 0)
			{
				printf("1");
				target_angle = 1 + (target_angle * 10);
			}
			else if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12) == 0)
			{
				printf("2");
				target_angle = 2 + (target_angle * 10);
			}
			else if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_11) == 0)
			{
				printf("3");
				target_angle = 3 + (target_angle * 10);
			}
			rows_init();
		}
		
		else if(GPIO_Pin == GPIO_PIN_6)
		{

			init_columns();
			
			if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_13) == 0)
			{
				printf("4");
				target_angle = 4 + (target_angle * 10);

			}
			else if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12) == 0)
			{
				printf("5");
				target_angle = 5 + (target_angle * 10);

			}
			else if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_11) == 0)
			{
				printf("6");
				target_angle = 6 + (target_angle * 10);

			}
			rows_init();
		}
		
		else if(GPIO_Pin == GPIO_PIN_8)
		{
			
			init_columns();
			
			if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_13) == 0)
			{
				__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_5);
				printf("7");
				target_angle = 7 + (target_angle * 10);

			}
			else if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12) == 0)
			{
				printf("8");
				target_angle = 8 + (target_angle * 10);
			}
			else if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_11) == 0)
			{
				printf("9");
				target_angle = 9 + (target_angle * 10);
			}
			
			rows_init();
		}
		
		else if(GPIO_Pin == GPIO_PIN_9)
		{
			
			init_columns();
			
			if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_13) == 0)
			{
				printf("*");
			}
			else if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12) == 0)
			{
				__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_4);
				printf("0");
				target_angle = 0 + (target_angle * 10);
			}
			else if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_11) == 0)
			{
				printf("#");
				enter = 1;
			}
			rows_init();
		}
		
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

