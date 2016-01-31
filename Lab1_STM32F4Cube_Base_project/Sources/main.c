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
	for(i = 0; i < Length; i++)
	{
		kstate->p = kstate->p + kstate->q;
		kstate->k = kstate->p/(kstate->p + kstate->r);
		kstate->x = kstate->x + (kstate->k * (InputArray[i] - kstate->x));
		kstate->p = (1 - kstate->k) * kstate->p;
		
		OutputArray[i] = kstate->x; 
	}
	
	return 0;
}

int main()
{
	float input[5] = {10.0, 12.0, 13.0, 14.0, 15.0};
	float output[5];
	int length = 5;
	
	int i;
	
	kalman_state ks;
	ks.q = 1;
	ks.r = 2;
	ks.x = 3;
	ks.p = 4;
	ks.k = 6;
	
	//float *a, *b;
	//a = &balance[0];
	//b = &balance[1];
	//Kalmanfilter_asm(&input[0], &output[0], &ks, length);
	
	Kalmanfilter_C(&input[0], &output[0], &ks, length);
	
	for(i = 0; i < 5; i++)
	{
		printf("%f\n", output[i]);
	}
	
	return 0;
}
