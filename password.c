/*
 * password.c
 *
 *  Created on: May 30, 2023
 *      Author: Edgar A. Mora
 */

#include "password.h"
#include "keypad.h"
#include "lcd.h"
#include "led.h"

// This function initializes the peripherals for password state
void password_init(void) {
	// Initializing Functions for LCD
	SysTick_Init();
	LCD_GPIO_init();
	LCD_init();
	LCD_home();
	// Initializing Keypad
	keypad_init();
}

// This function reads a 5-digit password for the system
// Returns a flag if password is correct [1] or not [0]
int password_type(void) {
	// System's Password is pre-defined
	int password[PASSWORD_LENGTH] = {9,3,4,0,7};
	// Initializing possible password
	int possible_pwd[PASSWORD_LENGTH] = {0,0,0,0,0};
	// Value of button press
	int button = -1;

	// Reading 5 numbers from keypad
	for (int count = 0; count < 5; count++) {
		button = keypad_decode();				// Read from keypad
		while (button == -1)					// Keep reading until a button is pressed
			button = keypad_decode();
		delay_us(150000);						// Delay for debouncing purposes
		possible_pwd[count] = button;			// Store the number in a 5-bit long array
		LCD_write_char('*');					// Display '*' on screen instead of pressed number
	} //end of for-loop

	// Checking if passwords match
	for (int i = 0; i < PASSWORD_LENGTH; i++) {
		// If there is at least one mismatched number, return an 'incorrect' flag [0]
		if (possible_pwd[i] != password[i])
			return 0;
	} //end of for-loop
	return 1; // Passwords matched, return 'correct' flag [1]
}

// This functions sets up the LCD typing password for the first time
void password_welcome(void){
	password_update();
	LED_toggle(2);    // Turn On Yellow LED
}

// This functions sets up the LCD typing password for all cases
void password_update(void){
	LCD_clear();
	LCD_home();
	LCD_write_string("ENTER PASSWORD:");
	// Cursor is sent to line 2 and waits for user input.
	LCD_second_line();
}

// This function displays on the LCD whether the input password is correct or not
// If incorrect 3-times in a row, LCD displays a security message.
// @arguments: attempts: Number of attempts left (MAX 5)
// 			   lock:     Flag for correct [1] or incorrect [0] password
void password_display(int attempts, int lock) {
	// Clears Screen
	LCD_clear();
	LCD_home();
	if (!lock) { // Incorrect Password
		LCD_write_string("WRONG PASSWORD!");
		LCD_second_line();
		LCD_write_number(attempts);
		LCD_write_string(" ATTEMPTS LEFT");
		// Keep the message up for a few seconds
		LED_toggle(0);		// Turn RED LED ON
		delay_us(6000000);
		LED_toggle(0);		// Turn RED LED OFF
	}

	else {	// Correct password
		LCD_write_string("PASSWORD IS");
		LCD_second_line();
		LCD_write_string("CORRECT!");
		// Keep the message up for a few seconds
		LED_toggle(2);    // Turn OFF Yellow LED
		LED_toggle(1);		// Turn GREEN LED ON
		delay_us(6000000);
	}
}
