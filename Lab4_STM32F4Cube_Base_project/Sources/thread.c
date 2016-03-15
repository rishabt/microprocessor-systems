#include "cmsis_os.h"                   // ARM::CMSIS:RTOS:Keil RTX
#include "stm32f4xx_hal.h"
#include "supporting_functions.h"
#include "lis3dsh.h"
#include "stm32f4xx_hal_gpio.h"
#include "math.h"
#include "thread.h"
#include "Kalmanfilter.h"
#include "accelerometer_interface.h"
#include "temperature_interface.h"


/**********************************************************************************************************
																				ACCELEROMETER
***********************************************************************************************************/

osThreadId tid_Thread_Accelerometer;
osThreadDef(Thread_Accelerometer, osPriorityNormal, 1, 0);

int start_Thread_Accelerometer (void) {

  tid_Thread_Accelerometer = osThreadCreate(osThread(Thread_Accelerometer ), NULL); // Start Accelerometer_Thread
  if (!tid_Thread_Accelerometer) return(-1); 
  return(0);
}


void Thread_Accelerometer (void const *argument) {
	while(1){
		accelerometer_mode();
	}
	
}


/**********************************************************************************************************
																				TEMPERATURE SENSOR
***********************************************************************************************************/

osThreadId tid_Temperature_Sensor;
osThreadDef(Thread_Temperature_Sensor, osPriorityNormal, 1, 0);

int start_Thread_Temperature_Sensor (void) {

  tid_Temperature_Sensor = osThreadCreate(osThread(Thread_Temperature_Sensor ), NULL); // Start LED_Thread
  if (!tid_Temperature_Sensor) return(-1); 
  return(0);
}


void Thread_Temperature_Sensor (void const *argument) {
	while(1){
		temperature_mode();
	}
	
}

