#include "accelerometer_interface.h"
#include "Kalmanfilter.h"

/*extern float readings[3];


kalman_state ks_x;
kalman_state ks_y;
kalman_state ks_z;

void KalmanState_Config(void){
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
*/