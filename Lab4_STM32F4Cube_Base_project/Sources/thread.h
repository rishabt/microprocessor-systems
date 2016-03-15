#ifndef THREAD_H
#define THREAD_H

int start_Thread_Accelerometer (void);

void accelerometer_set_semaphore(osSemaphoreId sem);

void Thread_Accelrometer (void const *argument);

void accelerometer_mode(void);

#endif
