#ifndef THREAD_H
#define THREAD_H


int start_Thread_Keypad (void);
void Thread_Keypad (void const *argument);

int start_Thread_Accelerometer (void);
void Thread_Accelerometer (void const *argument);

int start_Thread_Temperature_Sensor (void);
void Thread_Temperature_Sensor (void const *argument);

#endif
