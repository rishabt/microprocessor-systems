#ifndef ACC_H
#define ACC_H

#include "cmsis_os.h"                   // ARM::CMSIS:RTOS:Keil RTX


void accelerometer_set_semaphore(osSemaphoreId sem);

void acc_kalmanState_Config(void);

void accelerometer_mode(void);

void EXTI0_IRQHandler(void);

#endif
