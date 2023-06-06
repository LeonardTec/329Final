/*
 * led.h
 *
 *  Created on: May 31, 2023
 *      Author: Edgar A. Mora
 */

#ifndef INC_LED_H_
#define INC_LED_H_
#include "stm32l4xx_hal.h"

// MACRO DEFINITIONS
#define LED_PORT			GPIOF
#define LED_PORT_EN 		RCC_AHB2ENR_GPIOFEN

void LED_control_init(void);
void LED_toggle(int light);

#endif /* INC_LED_H_ */
