#include <stdio.h>
#include <stdlib.h>
#include "arm_math.h"

typedef struct kalman
{
	float q; // process noise covariance (covariance of noise(or interference) in actual (desired) state x)
	float r; // measurement noise covariance (covariance of noise in "sensor" measurement - assumed to have covariance of a zero mean Gaussian RV)
	float x; // value (state we're trying to determine)
	float p; // estimation error covariance (covariance of error between real sensor measurement and estimated sensor measurement using previously estimated state)
	float k; // kalman gain (gain applied to estimation error and added to previous state estimate to determine the present state estimate)
} kalman_state;


int Kalmanfilter(float* InputArray, float* OutputArray, kalman_state* kstate, int Length)
{
	int i;
	float tmp;
	
	if(InputArray == NULL || kstate == NULL || Length <= 0)	// Checks if any of the inputs is null or if length <= 0
	{
		return -1;
	}
	
	for(i = 0; i < Length; i++)
	{
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
		
		kstate->x = kstate->x + (kstate->k * (InputArray[i] - kstate->x));
		kstate->p = (1 - kstate->k) * kstate->p;
		
		OutputArray[i] = kstate->x; 
	}
	
	return 0;
}
