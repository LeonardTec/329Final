/*
 * keypad.h
 *
 *  Created on: May 13, 2023
 *      Author: Natalie Aynilian
 */

#ifndef INC_KEYPAD_H_
#define INC_KEYPAD_H_
#include "stm32l4xx_hal.h"

// MACRO DEFINITIONS
#define KEYPAD_PORT			GPIOC
#define KEYPAD_PORT_EN 		RCC_AHB2ENR_GPIOCEN

void keypad_init(void);
int keypad_decode(void);
int still_pressed(int prev_press);

#endif /* INC_KEYPAD_H_ */
