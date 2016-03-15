#include "temperature_interface.h"
#include "stm32f4xx.h"
#include "stm32f4xx_hal_gpio.h"
#include "Kalmanfilter.h"

extern TIM_HandleTypeDef TIM2_Handle;
extern float temp;
float temperature_reading;

osSemaphoreId temp_semaphore = NULL;

kalman_state temp_ks;

void temperature_set_semaphore(osSemaphoreId sem)
{
	temp_semaphore = sem;
}

void TIM2_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&TIM2_Handle);
}

void temperature_mode(void)
{
	
	osSemaphoreWait(temp_semaphore, osWaitForever);
	
	temperature_reading  = temp*(3.3f/ 4096.0f);											// ADC 3.3 Volts per 2^12 steps (12 bit resolution in configuration)
	temperature_reading -= (float)0.76;																// reference of 25C at 760mV
	temperature_reading /= (float)0.025;															// slope of 25mV/1C
	temperature_reading += 25;																				// add the reference offset back
	
	temperature_reading = Kalmanfilter(temperature_reading, &temp_ks);			// Use Kalman filter to handle noise
	printf("%f\n", temperature_reading);
}

void temp_kalmanState_Config(void){
	temp_ks.k = 0.0;
	temp_ks.p = 1500.0;
	temp_ks.q = 8;
	temp_ks.r = 100;
	temp_ks.x = 29.4;
}
