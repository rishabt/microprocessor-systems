#include "cmsis_os.h"                   // ARM::CMSIS:RTOS:Keil RTX
#include "stm32f4xx_hal.h"
#include "thread.h"

osThreadId tid_Thread_Accelerometer;
osThreadDef(Thread_Accelrometer, osPriorityNormal, 1, 0);

int start_Thread_Accelerometer (void) {

  tid_Thread_Accelerometer = osThreadCreate(osThread(Thread_Accelrometer ), NULL); // Start LED_Thread
  if (!tid_Thread_Accelerometer) return(-1); 
  return(0);
}


void Thread_Accelrometer (void const *argument) {
		while(1){
				
			}
}
