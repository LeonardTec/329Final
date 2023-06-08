/*
 * control.c
 *
 *  Created on: Jun 6, 2023
 *      Author: Edgar A. Mora
 */

#include "password.h"
#include "lcd.h"
#include "control.h"
#include "keypad.h"

// Function to display ROOM SELECTION
void control_room_select(void) {
	// Clears Screen
	LCD_clear();
	LCD_home();
	// ROOM SELECTION SCREEN
	LCD_write_string("SELECT A ROOM:");
	LCD_second_line();
	LCD_write_string(" RM1  RM2  RM3 ");
}

// Function that polls for a keypad entry
int control_keypad(void) {
	int button = -1;
	button = keypad_decode();				// Read from keypad
	while (button == -1)					// Keep reading until a button is pressed
		button = keypad_decode();
	delay_us(150000);
	return button;
}

// Function that displays settings for ROOM 2 or ROOM3
void control_light2or3(void) {
	// Clears Screen
	LCD_clear();
	LCD_home();
	// Display Options
	LCD_write_string("1:ON     2:OFF");
	LCD_second_line();
	LCD_write_string("0:RETURN");
}

// Function that displays commands for LIGHTING in ROOM 1
void control_light1(void) {
	// Clears Screen
	LCD_clear();
	LCD_home();
	// Display Options
	LCD_write_string("1:ON     2:OFF");
	LCD_second_line();
	LCD_write_string("0:RETURN 9:HOME");
}

// Function for displaying settings at ST_ROOM1
void control_room1_display(void) {
	// Clears Screen
	LCD_clear();
	LCD_home();
	// Display Options
	LCD_write_string("1:LIGHTS   2:FAN");
	LCD_second_line();
	LCD_write_string("9:HOME");
}

// Function that displays commands for Brighting/Dimming
void control_brightness(void) {
	// Clears Screen
	LCD_clear();
	LCD_home();
	// Display Options
	LCD_write_string("1:BRIGHT 2:DIM");
	LCD_second_line();
	LCD_write_string("0:RETURN 9:HOME");

}

// // Function that displays commands for FAN
void control_fan(void) {
	// Clears Screen
	LCD_clear();
	LCD_home();
	// Display Options
	LCD_write_string("1:ON/OFF");
	LCD_second_line();
	LCD_write_string("0:RETURN 9:HOME");
}

// Function that initializes FAN GPIO PIN
void control_fan_init(void) {
	// Enable Clocks
	RCC->AHB2ENR |= MOTOR_PORT_EN;

	// Set GPIO Mode to Output
	MOTOR_PORT -> MODER   &= ~(GPIO_MODER_MODE0);
	MOTOR_PORT -> MODER   |= GPIO_MODER_MODE0_0;

	// Push-Pull Reset
	MOTOR_PORT -> OTYPER  &= ~( GPIO_OTYPER_OT0 );

	// No pull up, pull down
	MOTOR_PORT -> PUPDR  &= ~(GPIO_PUPDR_PUPD0);

	// Highest Speed
	MOTOR_PORT-> OSPEEDR |=  (GPIO_OSPEEDR_OSPEED0_0 | GPIO_OSPEEDR_OSPEED0_1);

	// Start with MOTOR OFF
	MOTOR_PORT -> ODR &= ~(GPIO_PIN_0);
}
