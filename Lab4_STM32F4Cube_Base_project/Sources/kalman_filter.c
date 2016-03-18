/**
 * @file kalmanfilter.h
 * @brief Kalmanfilter fot the lab
 **/
 
#include <stdio.h>
#include <stdlib.h>
#include "Kalmanfilter.h"
#include "cmsis_os.h"                   // ARM::CMSIS:RTOS:Keil RTX

extern osMutexId kalman_filter_mutex;

float Kalmanfilter(float Input, kalman_state* kstate)
{
	float tmp;
	osMutexWait(kalman_filter_mutex, osWaitForever);
	
	kstate->p = kstate->p + kstate->q;
	tmp = (kstate->p + kstate->r);
	if(tmp != 0)	// Check to avoid getting NaN on division
	{
		kstate->k = kstate->p/(kstate->p + kstate->r);
	}
	else
	{
		return -1;
	}
	
	kstate->x = kstate->x + (kstate->k * (Input - kstate->x));
	kstate->p = (1 - kstate->k) * kstate->p;
	osMutexRelease(kalman_filter_mutex);
	return kstate->x; 
}
