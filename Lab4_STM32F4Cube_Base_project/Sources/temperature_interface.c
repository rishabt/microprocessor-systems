#include "temperature_interface.h"
#include "stm32f4xx.h"
#include "stm32f4xx_hal_gpio.h"
#include "Kalmanfilter.h"

extern TIM_HandleTypeDef TIM2_Handle;

extern float temp; 								// Temperature polled inside the TIM2 callback
float temperature_reading;				// Filtered "temp" temperature reading
extern double tmp_temperature;		// Temperature value to be displayed (hard-coded to 30 degrees... just kidding!)
int RAISE_ALARM = 0;							// Overheat alarm 1:on/0:off
extern int display_flag;					// External counter used to create a frequency divider for updating tmp_temperature
extern osMutexId display_flag_mutex;

osSemaphoreId temp_semaphore = NULL;	// Semaphore used for synchronization of thread with TIM2 callback

kalman_state temp_ks;									// Kalman state for kalman filter

void temperature_set_semaphore(osSemaphoreId sem) {
	temp_semaphore = sem;
}

/*
 * Timer interrupt request received to poll
 * ADC. (Callback implemented in main.c)
*/
void TIM2_IRQHandler(void) {
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

	if (temperature_reading > 28)
		RAISE_ALARM = 1;
	else
		RAISE_ALARM = 0;
	
	osMutexWait(display_flag_mutex, osWaitForever);
	if(display_flag % 300 == 0){			
		tmp_temperature = temperature_reading;
	}
	osMutexRelease(display_flag_mutex);

}

void temp_kalmanState_Config(void){
	temp_ks.k = 0.0;
	temp_ks.p = 1500.0;
	temp_ks.q = 8;
	temp_ks.r = 100;
	temp_ks.x = 29.4;
}
