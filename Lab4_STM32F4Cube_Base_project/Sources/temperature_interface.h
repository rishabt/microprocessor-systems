#ifndef TEMP_H
#define TEMP_H

#include "cmsis_os.h"

void temperature_set_semaphore(osSemaphoreId sem);
	
void TIM2_IRQHandler(void);

void temperature_mode(void);

void temp_kalmanState_Config(void);

#endif
