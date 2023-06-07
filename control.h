/*
 * control.h
 *
 *  Created on: Jun 6, 2023
 *      Author: Edgar A. Mora
 */

#ifndef INC_CONTROL_H_
#define INC_CONTROL_H_


#include "stm32l4xx_hal.h"
// MACRO Definitions
#define MAX_BRIGHTNESS 4800;
#define MIN_BRIGHTNESS 2600;
#define MOTOR_PORT GPIOG
#define MOTOR_PORT_EN RCC_AHB2ENR_GPIOGEN
//#define BRIGHT_CHANGE  200;

// FUNCTION PROTOTYPES
void control_room_select(void);
void control_room1_display(void);
void control_light2or3(void);
void control_light1(void);
void control_brightness(void);
void control_fan(void);
void control_fan_init(void);
int  control_keypad(void);
#endif /* INC_CONTROL_H_ */
