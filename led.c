/*
 * led.c
 *
 *  Created on: May 31, 2023
 *      Author: Edgar A. Mora
 */

#include "led.h"

// This function initializes the 4 LEDS for displaying
// PF0: Red- Incorrect Password
// PF1:	Green- Correct Password
// PF2: Yellow- Input Password Stage
// PF3: Blue- Warning
void LED_control_init(void) {
	// Clock enable for GPIOE
	RCC->AHB2ENR   |= (LED_PORT_EN);
	// Set PF0,PF1,PF2,PF3 Ports
	// Clear contents of PF0-PF3
	LED_PORT->MODER &= ~(GPIO_MODER_MODE0|GPIO_MODER_MODE1|GPIO_MODER_MODE2|GPIO_MODER_MODE3);
	// Set PF0-PF3 to General Purpose Output Mode [0 1]
	LED_PORT->MODER |= (GPIO_MODER_MODE0_0|GPIO_MODER_MODE1_0|GPIO_MODER_MODE2_0|GPIO_MODER_MODE3_0);
	// Set PF0-PF3 to push/pull Mode
	LED_PORT->OTYPER  &= ~(GPIO_OTYPER_OT0 | GPIO_OTYPER_OT1|GPIO_OTYPER_OT2 | GPIO_OTYPER_OT3);
	// Set PF0-PF3 to No pull up, No pull down Mode [0 0]
	LED_PORT->PUPDR  &= ~(GPIO_PUPDR_PUPD0 | GPIO_PUPDR_PUPD1 |GPIO_PUPDR_PUPD2 |GPIO_PUPDR_PUPD3);
	// Clearing contents of PF0-PF3 OSPEEDR Register
	LED_PORT->OSPEEDR &= ~( GPIO_OSPEEDR_OSPEED0 |GPIO_OSPEEDR_OSPEED1 |GPIO_OSPEEDR_OSPEED2 |GPIO_OSPEEDR_OSPEED3 );
	// Setting PF0-PF3 to Highest Speed Mode [1 1]
	LED_PORT->OSPEEDR |=  ( GPIO_OSPEEDR_OSPEED0_0 | GPIO_OSPEEDR_OSPEED0_1 |
		    				GPIO_OSPEEDR_OSPEED1_0 | GPIO_OSPEEDR_OSPEED1_1 |
							GPIO_OSPEEDR_OSPEED2_0 | GPIO_OSPEEDR_OSPEED2_1 |
							GPIO_OSPEEDR_OSPEED3_0 | GPIO_OSPEEDR_OSPEED3_1 );
	// Turn OFF all LEDs
	LED_PORT -> ODR &= ~(GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2);
}

// This function toggles the control LED
// light variable determines which LED to toggle
void LED_toggle(int light){
	switch(light) {
	case 0:
		LED_PORT-> ODR ^= (GPIO_PIN_0);
		break;
	case 1:
		LED_PORT-> ODR ^= (GPIO_PIN_1);
		break;
	case 2:
		LED_PORT-> ODR ^= (GPIO_PIN_2);
		break;
	case 3:
		LED_PORT-> ODR ^= (GPIO_PIN_3);
		break;
	default:
		LED_PORT-> ODR ^= (GPIO_PIN_0);
		break;
	}
}
