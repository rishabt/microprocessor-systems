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

#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))

extern int Kalmanfilter_asm(float* InputArray, float* OutputArray, kalman_state* kstate, int Length);
/*
 * Implementation of 1D KalmanFilter
 * @input 
 */
int Kalmanfilter_C(float* InputArray, float* OutputArray, kalman_state* kstate, int Length)
{
	int i;
	float tmp;
	
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
  variance = (sq_sum/Length) - mean*mean;																// Main calculation of the variance using formula (mean of sum of squares - square of mean)
	std_deviation = sqrt(variance);																				// Main calculation of standard deviation using formula sqrt(variance)			
	
	StatsArray[0] = mean;
	StatsArray[1] = std_deviation;
	
	return StatsArray;
}


/*
 * Calculates the correlation coefficient
*/
float* calculate_correlation(float* Vector_A, float* Vector_B, float* CorrelationArray, int Length_A, int Length_B) 
{
	int Length_Correlation = 2* MAX(Length_A,Length_B) - 1;
	int n, reverse;
	int tempLength = 0;
	
	for (n = 0; n < Length_Correlation; n++)
	{
		int k = 0;
		if(n < Length_A)
		{
			tempLength++;
		} else
		{
			reverse = 1;
			tempLength--;
		}
		
		while (k < tempLength)
		{
			if (reverse == 0)
			{
				CorrelationArray[n] += Vector_A[k]*Vector_B[Length_B - tempLength + k];
			} 
			else
			{
				CorrelationArray[n] += Vector_A[Length_A-tempLength+k]*Vector_B[k];
			}
			k++;
		}
	}
	
	return CorrelationArray;
	
}

/*
 * Performs convolution on 1D arrays
 * Length of output = length of input_a + length of input_b - 1
 */
float* calculate_convolution(float* Vector_A, float* Vector_B, float* ConvolutionArray, int Length_A, int Length_B)
{	
	int i;
	int Length_Output = 2* MAX(Length_A,Length_B) - 1;
  for (i = 0; i < Length_Output; i++)
  {
    int kmin, kmax, k;																									// Variables to control the loop calculating convolution

    ConvolutionArray[i] = 0;

    kmin = (i >= Length_A - 1) ? i - (Length_B - 1) : 0;								// If i >= length_a - 1 then kmin = i - (length_b - 1) else kmin = 0
    kmax = (i < Length_A - 1) ? i : Length_B - 1;												// If i < length_a - 1 then kmax = i else kmax = length_b - 1

    for (k = kmin; k <= kmax; k++)																			// Main loop that performs convolution and populates the OutputArray
    {
      ConvolutionArray[i] += Vector_A[k] * Vector_B[i - k];
    }
  }
	
	return ConvolutionArray;
}

/*
 * CMSIS-DSP implementation of subtration
 */
float* subtract_cmis(float* InputArray, float* OutputArray, float* DifferenceArray, int Length){	
	
	arm_sub_f32(InputArray, OutputArray, DifferenceArray, Length);
	return DifferenceArray;
}

/*
 * CMSIS-DSP function to calculate the mean and standard deviation of the difference array
 * StatsArray[0] -> mean
 * StatsArray[1] -> standard deviation
 */
float* calculate_stats_cmis(float* DifferenceArray, float* StatsArray, int Length){
  arm_mean_f32(DifferenceArray, Length, &StatsArray[0]);
	arm_std_f32(DifferenceArray, Length, &StatsArray[1]);
	
	return StatsArray;
}

/*
 * CMSIS implementation of correlation
 */
float* calculate_correlation_cmis(float* InputArray, float* OutputArray, float* CorrelationArray, int Length_Input, int Length_Output){
	
	arm_correlate_f32(InputArray, Length_Input, OutputArray, Length_Output, CorrelationArray);
	return CorrelationArray;
}

/*
 * CMSIS implementation of convolution
 * Length of output = length of input_a + length of input_b - 1
 */
float* calculate_convolution_cmis(float* InputArray, float* OutputArray, float* DestinationArray, int Length_Input, int Length_Output){
	
	arm_conv_f32(InputArray, Length_Input, OutputArray, Length_Output, DestinationArray);
	return DestinationArray;
}

void print_array(float* array, int length)
{
	int i;
	//call kalman filter
	for(i = 0; i < length; i++)
	{
			printf("%f\n", array[i]);
	}
}

int main(){
	
	/*----------------------INIT----------------------*/
	int length = 4;
	float input[4] = {-1, 0.125, 31.0, 1.0625};
	float output[4];
	float corr[7];
	float corr_cmis[7];
	float conv[7];
	float conv_cmis[7];
	float diff[4];
	float diff_cmis[4];
	float stats[2];
	float stats_cmis[2];

	int resultStatus = 0;

	kalman_state ks;
	ks.q = 0.1;
	ks.r = 0.1;
	ks.x = 0;
	ks.p = 0.1;
	ks.k = 0;
	/*-------------------------------------------------*/

	/*----------------------Filter Call-----------------*/

	resultStatus = Kalmanfilter_asm(&input[0], &output[0], &ks, length);
		printf("Output Array \n");
		print_array(&output[0], length);
		printf("\n\n");
	resultStatus = Kalmanfilter_C(&input[0], &output[0], &ks, length);

	subtract(&input[0], &output[0], &diff[0], length);
	subtract_cmis(&input[0], &output[0], &diff_cmis[0], length);
	calculate_stats(&diff[0], &stats[0], length);
	calculate_stats_cmis(&diff_cmis[0], &stats_cmis[0], length);
	calculate_correlation(&input[0], &output[0], &corr[0], length, length);
	calculate_correlation_cmis(&input[0], &output[0], &corr_cmis[0], length, length);
	calculate_convolution(&input[0], &output[0], &conv[0], length, length);
	calculate_convolution_cmis(&input[0], &output[0], &conv_cmis[0], length, length);
	/*--------------------------------------------------*/

	
	if(resultStatus != -1)
	{
		printf("Output Array \n");
		print_array(&output[0], length);
		printf("\n\n");
		printf("Correlation Array \n");
		print_array(&corr[0], 7);
		printf("\n\n");
		printf("Correlation Array in CMIS\n");
		print_array(&corr_cmis[0], 7);
		printf("\n\n");
		printf("Convolution Array \n");
		print_array(&conv[0], 7);
		printf("\n\n");
		printf("Convolution Array in CMIS\n");
		print_array(&conv_cmis[0], 7);
		printf("\n\n");
		printf("Difference Array \n");
		print_array(&diff[0], length);
		printf("\n\n");
		printf("Difference Array in CMIS\n");
		print_array(&diff_cmis[0], length);
		printf("\n\n");
		printf("Stats Array \n");
		print_array(&stats[0], 2);
		printf("\n\n");
		printf("Stats Array in CMIS\n");
		print_array(&stats_cmis[0], 2);
		printf("DID WE GET IT??\n");
	}
	
	return 0;
}
