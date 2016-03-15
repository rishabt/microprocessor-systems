#include "accelerometer_interface.h"
#include "Kalmanfilter.h"
#include "RTE_Components.h"             // Component selection
#include "math.h"
#include "stm32f4xx.h"
#include "stm32f4xx_hal_gpio.h"

extern float readings[3];
float pitch;

osSemaphoreId semaphore = NULL;

kalman_state ks_x;
kalman_state ks_y;
kalman_state ks_z;

void accelerometer_set_semaphore(osSemaphoreId sem)
{
	semaphore = sem;
}

void acc_kalmanState_Config(void){
	ks_x.k = 0.0;
	ks_x.p = 1500.0;
	ks_x.q = 8;
	ks_x.r = 100;
	ks_x.x = 29.4;

	ks_y.k = 0.0;
	ks_y.p = 1500.0;
	ks_y.q = 8;
	ks_y.r = 100;
	ks_y.x = 29.4;
	
	ks_z.k = 0.0;
	ks_z.p = 1500.0;
	ks_z.q = 8;
	ks_z.r = 100;
	ks_z.x = 29.4;
}

float misalignment_and_offset_matrix[4][3] = {
	{0.9932,-0.0189,0.0662},
	{0.0170, 0.9770,0.0493},
	{-0.0026,-0.01717,0.9498},
	{0.0023, -0.0051,-0.0495}
};

void accelerometer_mode(void)
{	
	float x,y,z;
	float raw_matrix[4] = {0};
	float callibrated_matrix[3] = {0};
	int i,j;
	
	osSemaphoreWait(semaphore, osWaitForever);
	
	//LIS3DSH_ReadACC(readings);
	
	raw_matrix[0] = readings[0];
	raw_matrix[1] = readings[1];
	raw_matrix[2] = readings[2];
	raw_matrix[3] = 1;

	for (i=0; i<3; i++){
		for (j=0; j<4; j++){
			callibrated_matrix[i] += raw_matrix[j]*misalignment_and_offset_matrix[j][i];
		}
	}
	
	x = Kalmanfilter(callibrated_matrix[0],&ks_x);
	y = Kalmanfilter(callibrated_matrix[1],&ks_y);
	z = Kalmanfilter(callibrated_matrix[2],&ks_z);
	
	pitch = atan2(x, sqrt(y*y + z*z)) * 180/ 3.14159265;
	
	printf("%f\n", pitch);
	
}

void EXTI0_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
}
