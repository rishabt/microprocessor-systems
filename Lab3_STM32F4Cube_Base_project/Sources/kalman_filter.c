/**
 * @file kalmanfilter.h
 * @brief Kalmanfilter fot the lab
 **/
 
#include <stdio.h>
#include <stdlib.h>
#include "kalman_filter.h"

float Kalmanfilter(float Input, kalman_state* kstate)
{
	float tmp;
	
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
	
	return kstate->x; 
}
