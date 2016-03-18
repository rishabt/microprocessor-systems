#ifndef KEYPAD_H
#define KEYPAD_H

#include "cmsis_os.h"                   // ARM::CMSIS:RTOS:Keil RTX

void keypad_set_semaphore(osSemaphoreId sem);

void keypad_mode(void);

void EXTI9_5_IRQHandler(void);

/**
 * @brief Initialize rows as input and columns as output
 * 
 * @param Void
 * @return Void
 **/
void rows_init(void);

/**
 * @brief Initialize columns as input and rows as output
 * 
 * @param Void
 * @return Void
 **/
void init_columns(void);

#endif