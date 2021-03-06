/**
 * @file kalmanfilter.h
 * @brief Kalmanfilter fot the lab
 **/

#ifndef KALMAN_H
#define KALMAN_H

typedef struct kalman
{
	float q; // process noise covariance (covariance of noise(or interference) in actual (desired) state x)
	float r; // measurement noise covariance (covariance of noise in "sensor" measurement - assumed to have covariance of a zero mean Gaussian RV)
	float x; // value (state we're trying to determine)
	float p; // estimation error covariance (covariance of error between real sensor measurement and estimated sensor measurement using previously estimated state)
	float k; // kalman gain (gain applied to estimation error and added to previous state estimate to determine the present state estimate)
} kalman_state;

/**
 * @brief Kalmanfilter 
 * @param Input Input number
 * @param kstate Kalman state
 * @return Void
 **/
float Kalmanfilter(float Input, kalman_state* kstate);

#endif
