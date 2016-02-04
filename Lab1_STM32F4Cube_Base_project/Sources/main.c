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
/*
 * Implementation of 1D KalmanFilter
 * @input 
 */
int Kalmanfilter_C(float* InputArray, float* OutputArray, kalman_state* kstate, int Length)
{
	int i;
	int tmp;
	
	if(InputArray == NULL || kstate == NULL || Length <= 0)									// Checks if any of the inputs is null or if length <= 0
	{
		return -1;
	}
	
	for(i = 0; i < Length; i++)
	{
		kstate->p = kstate->p + kstate->q;
		
		tmp = (kstate->p + kstate->r);
		if(tmp != 0)																													// Check to avoid getting NaN on division
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

/*
 * Subtracts the results of the two arrays and stores them in a difference array
 */
float* subtract(float* InputArray, float* OutputArray, float* DifferenceArray, int Length)
{	
	int i;
	for(i = 0; i < Length; i++)																						// Iterates and calculates difference for each element
	{
		DifferenceArray[i] = InputArray[i] - OutputArray[i];
	}
	
	return DifferenceArray;
}

/*
 * CMIS-DSP implementation of subtration
 */
float* subtract_cmis(float* InputArray, float* OutputArray, float* DifferenceArray, int Length)
{	
	 arm_sub_f32(InputArray, OutputArray, DifferenceArray, Length);
	
	return DifferenceArray;
}

/*
 * Calculates the mean, variance and standard deviation of the difference array
 * StatsArray[0] -> mean
 * StatsArray[1] -> standard deviation
 */
float* calculate_stats(float* DifferenceArray, float* StatsArray, int Length)
{
  float sum = 0;
  float sq_sum = 0;
	
	float mean;
  float variance;
	float std_deviation;
	
	int i;
  for(i = 0; i < Length; ++i) 
	{
		sum += DifferenceArray[i];																					// Calculates the sum of elements
    sq_sum += DifferenceArray[i] * DifferenceArray[i];									// Calculates the sum of squares
  }
	
  mean = sum / Length;																									// Main calculation of the mean using formula sum/length
  variance = sq_sum / Length - mean * mean;															// Main calculation of the variance using formula (sum of squares)/length * mean^2
	std_deviation = sqrt(variance);																				// Main calculation of standard deviation using formula sqrt(variance)			
	
	StatsArray[0] = mean;
	StatsArray[1] = std_deviation;
	
	return StatsArray;
}

/*
 * CMIS-DSP function to calculate the mean and standard deviation of the difference array
 * DifferenceArray[0] -> mean
 * DifferenceArray[2] -> standard deviation
 */
float* calculate_stats_cmis(float* DifferenceArray, float* StatsArray, int Length)
{
  arm_mean_f32(DifferenceArray, Length, &StatsArray[0]);
	arm_std_f32(DifferenceArray, Length, &StatsArray[1]);
	
	return StatsArray;
}

/*
 * Calculates the correlation coefficient
 */
float calculate_correlation(float* InputArray, float* OutputArray, int Length)
{
	float correlation;
	
	float sum_inputs;
	float sum_outputs;
	float input_sq_sum;
	float output_sq_sum;
	float sum_of_products;
	
	float s_ii;
	float s_oo;
	float s_io;	
	
	if(InputArray == NULL || Length <= 0)																	// Checks for null inputs and length <= 0
	{
		return -1;
	}
	
	int i;
	for(i = 0; i < Length; i++)
	{
		sum_inputs += InputArray[i];																				// Accumulates sum of input array elements
		sum_outputs += OutputArray[i];																			// Accumulates sum of output array elements
		input_sq_sum += InputArray[i] * InputArray[i];											// Accumulates sum of squares of input array elements
		output_sq_sum += OutputArray[i] * OutputArray[i];										// Accumulates sum of squares of output array elements
		sum_of_products += InputArray[i] * OutputArray[i];									// Accumulates sum of product of corresponding input and output array elements
	}
	
	s_ii = input_sq_sum - sum_inputs*sum_inputs/Length;										
	s_oo = output_sq_sum - sum_outputs*sum_outputs/Length;
	
	s_io = sum_of_products - sum_inputs*sum_outputs/Length;
	
	correlation = s_io/sqrt(s_ii*s_oo);																		// Main calculation of correlation coefficient
	
	return correlation;
}

/*
 * CMIS implementation of correlation
 */
float* calculate_correlation_cmis(float* InputArray, float* OutputArray, float* CorrelationArray, int Length_Input, int Length_Output, int Length)
{
	arm_correlate_f32(InputArray, Length_Input, OutputArray, Length_Output, CorrelationArray);
	
	return CorrelationArray;
}

/*
 * Performs convolution on 1D arrays
 * Length of output = length of input_a + length of input_b - 1
 */
float* find_convolution(float* Vector_A, float* Vector_B, float* OutputArray, int Length_A, int Length_B, int Length_Output)
{	
	int i;
  for (i = 0; i < Length_Output; i++)
  {
    int kmin, kmax, k;																									// Variables to control the loop calculating convolution

    OutputArray[i] = 0;

    kmin = (i >= Length_A - 1) ? i - (Length_B - 1) : 0;								// If i >= length_a - 1 then kmin = i - (length_b - 1) else kmin = 0
    kmax = (i < Length_A - 1) ? i : Length_B - 1;												// If i < length_a - 1 then kmax = i else kmax = length_b - 1

    for (k = kmin; k <= kmax; k++)																			// Main loop that performs convolution and populates the OutputArray
    {
      OutputArray[i] += Vector_A[k] * Vector_B[i - k];
    }
  }
	
	return OutputArray;
}

/*
 * CMIS implementation of convolution
 * Length of output = length of input_a + length of input_b - 1
 */
float* calculate_convolution_cmis(float* InputArray, float* OutputArray, float* DestinationArray, int Length_Input, int Length_Output, int Length)
{
	arm_conv_f32(InputArray, Length_Input, OutputArray, Length_Output, DestinationArray);
	
	return DestinationArray;
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
	
	if(result != -1)
	{
		for(i = 0; i < 5; i++)
		{
			printf("%f\n", output[i]);
		}
	}
	
	return 0;
}
