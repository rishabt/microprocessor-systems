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

extern int Kalmanfilter_asm(float* InputArray, float* OutputArray, kalman_state* kstate, int Length);

int Kalmanfilter_C(float* InputArray, float* OutputArray, kalman_state* kstate, int Length)
{
	int i;
	int tmp;
	
	if(InputArray == NULL || kstate == NULL || Length <= 0)
	{
		return 0;
	}
	
	for(i = 0; i < Length; i++)
	{
		kstate->p = kstate->p + kstate->q;
		
		tmp = (kstate->p + kstate->r);
		if(tmp != 0)
		{
			kstate->k = kstate->p/(kstate->p + kstate->r);
		}
		else
		{
			return 0;
		}
		
		kstate->x = kstate->x + (kstate->k * (InputArray[i] - kstate->x));
		kstate->p = (1 - kstate->k) * kstate->p;
		
		OutputArray[i] = kstate->x; 
	}
	
	return 1;
}

int main()
{
	float input[4] = {-1, 0.125, 31.0, 1.0625};
	float output[4];
	int length = 4;
	
	int i;
	int result;
	
	kalman_state ks;
	ks.q = -15.5;
	ks.r = 2.25;
	ks.x = -0.59375;
	ks.p = -0.296875;
	ks.k = -1.3125;
	
	//float *a, *b;
	//a = &balance[0];
	//b = &balance[1];
	//Kalmanfilter_asm(&input[0], &output[0], &ks, length);
	
	result = Kalmanfilter_C(&input[0], &output[0], &ks, length);
	
	if(result != 0)
	{
		for(i = 0; i < 5; i++)
		{
			printf("%f\n", output[i]);
		}
	}
	
	return 0;
}
