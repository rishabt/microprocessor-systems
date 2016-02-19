/**
 * @file led_interface.h
 * @brief Interface for LED. Functions in this file 
 * 				are used for switching on/off LEDs and 
 *				raising alarm in high temperature conditions
 **/
#ifndef LED_H
#define LED_H

/**
 * @brief Turns green LED on
 * @param
 * @return
 **/
void light_green(void);

/**
 * @brief Turns green LED off
 * @param
 * @return
 **/
void disable_green(void);

/**
 * @brief Turns orange LED on
 * @param
 * @return
 **/
void light_orange(void);

/**
 * @brief Turns orange LED off
 * @param
 * @return
 **/
void disable_orange(void);

/**
 * @brief Turns red LED on
 * @param
 * @return
 **/
void light_red(void);

/**
 * @brief Turns red LED off
 * @param
 * @return
 **/
void disable_red(void);

/**
 * @brief Turns blue LED on
 * @param
 * @return
 **/
void light_blue(void);

/**
 * @brief Turns blue LED off
 * @param
 * @return
 **/
void disable_blue(void);	

/**
 * @brief Raise alarm in case of high temperatures
 * @param
 * @return
 **/
void raise_alarm(void);

/**
 * @brief Switch off alarm
 * @param
 * @return
 **/
void switch_off_alarm(void);


#endif
