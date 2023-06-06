/*
 * keypad.c
 *
 * Description:
 * Driver for 4X3 Keypad (Designed for use with STM NUCLEO L496ZG)
 * Waits for button press by checking input ROWS.
 * Once button press is detected, check to see
 * which button was pressed by checking columns.
 * ----------------------------------------------------------------
 * Pin out:
 * ROW 0 - PC_0			COL 0 - PC_4
 * ROW 1 - PC_1			COL 1 - PC_5
 * ROW 2 - PC_2			COL 2 - PC_6
 * ROW 3 - PC_3
 *
 * Pins: |UNUSED|Column1|Row0|Column0|Row3|Column2|Row2|Row1|UNUSED|
 * GPIO: |UNUSED|  PC5  |PC0 |  PC4  |PC3 |  PC6  |PC2 |PC1 |UNUSED|
 * ----------------------------------------------------------------
 *
 *  CAL POLY SLO
 *  Instructor: Jason Poon
 *  Date: 4/10/2023 (V1)
 *  	  4/13/2023 (V2) - Added de-bounce functionality
 *  	  5/30/2023 (V3) - Making a MACROS for GPIOs
 *  Authors: Natalie Aynilian, Patrick Jackson
 *  Citation: Sebastian Rosso assisted with PIN configuration process
 */

#include "keypad.h"
#include <math.h>

int ROWS = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3; // ROW 0, ROW 1, ROW 2, ROW 3
int COLS = GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6; // COL 0, COL 1, COL 2

// Initialize all Pin Configurations for Keypad
void keypad_init(){
	RCC->AHB2ENR |= (KEYPAD_PORT_EN); // Enable Clock for GPIO (Port C)

	/* KEYPAD COLUMNS */
	// Configure GPIO Pin Modes
	// Clears GPIO C0-C11 (Sets rows (C0-C3) to input mode)
	KEYPAD_PORT->MODER &= GPIO_MODER_MODE12 | GPIO_MODER_MODE13 | GPIO_MODER_MODE14 | GPIO_MODER_MODE15;

	//Enable column pins as output mode
	KEYPAD_PORT->MODER |= GPIO_MODER_MODE4_0 | GPIO_MODER_MODE5_0 | GPIO_MODER_MODE6_0;

	// Columns are in No Pull up, No pull down Mode [0 0]
	KEYPAD_PORT->PUPDR  &= ~(GPIO_MODER_MODE4 |GPIO_MODER_MODE5 |GPIO_MODER_MODE6 );

	//Setting Columns to push/pull Mode [0]
	KEYPAD_PORT->OTYPER &= ~(GPIO_OTYPER_OT4 | GPIO_OTYPER_OT5 | GPIO_OTYPER_OT6 );

	// Clearing contents of Columns OSPEEDR Register
	KEYPAD_PORT->OSPEEDR &= ~(GPIO_OSPEEDR_OSPEED4 | GPIO_OSPEEDR_OSPEED5 | GPIO_OSPEEDR_OSPEED6 );

	// Setting Columns to Highest Speed Mode [1 1]
	KEYPAD_PORT->OSPEEDR |=  (GPIO_OSPEEDR_OSPEED4_0 | GPIO_OSPEEDR_OSPEED4_1 |
	    					  GPIO_OSPEEDR_OSPEED5_0 | GPIO_OSPEEDR_OSPEED5_1 |
							  GPIO_OSPEEDR_OSPEED6_0 | GPIO_OSPEEDR_OSPEED6_1 );

	// Clears Pull Up/Down Register for Row Pins
	KEYPAD_PORT->PUPDR &= ~(GPIO_PUPDR_PUPD0 | GPIO_PUPDR_PUPD1 | GPIO_PUPDR_PUPD2 | GPIO_PUPDR_PUPD3);
	// Sets rows to pull down resistors
	KEYPAD_PORT->PUPDR |= GPIO_PUPDR_PUPD0_1 | GPIO_PUPDR_PUPD1_1 | GPIO_PUPDR_PUPD2_1 | GPIO_PUPDR_PUPD3_1;

	//Setting Rows to push/pull Mode [0]
	KEYPAD_PORT->OTYPER &= ~(GPIO_OTYPER_OT0 |GPIO_OTYPER_OT1 |GPIO_OTYPER_OT2 |GPIO_OTYPER_OT3);

	// Clearing contents of Rows OSPEEDR Register
	KEYPAD_PORT->OSPEEDR &= ~(GPIO_OSPEEDR_OSPEED0 |GPIO_OSPEEDR_OSPEED1 |GPIO_OSPEEDR_OSPEED2 |GPIO_OSPEEDR_OSPEED3);

	// Setting Rows to Highest Speed Mode [1 1]
	KEYPAD_PORT->OSPEEDR |=  (GPIO_OSPEEDR_OSPEED0_0 | GPIO_OSPEEDR_OSPEED1_1 |
	    					  GPIO_OSPEEDR_OSPEED1_0 | GPIO_OSPEEDR_OSPEED2_1 |
							  GPIO_OSPEEDR_OSPEED2_0 | GPIO_OSPEEDR_OSPEED3_1 |
							  GPIO_OSPEEDR_OSPEED3_0 | GPIO_OSPEEDR_OSPEED4_1);

	// Set all columns to high
	KEYPAD_PORT->BSRR = GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6;
}

// De-bounce
// Parameter: prev_press - Keypad press from keypad_decode
// Return: Keypad press from keypad_decode (after checking for faulty press)
int still_pressed(int prev_press)
{
	int isPressed; //Set to 1 if there was a keypad press
	int current_press; // Current decoded keypad press

	//No keypad button was pressed (Do nothing)
	if(prev_press == -1)
	{
		return prev_press;
	}

	isPressed = 1;

	// While there is a keypad press:
	// Check if it is the same press to avoid faulty press
	while(isPressed)
	{
		for(int i; i < 1000; i++); //Delay (2.2 ms)

		current_press = keypad_decode();

		if(prev_press != current_press)
		{
			isPressed = 0;
		}
	}

	return prev_press;
}

// Decodes keypad press from rows and columns
// Return: keypad press value (-1 for no keypad press)
int keypad_decode(){

	int row_press = 0;

	KEYPAD_PORT->BSRR = COLS; // SET COLUMNS HIGH (To detect a row press)

	// Determine IF button was pressed (detect a row going high)
	if(KEYPAD_PORT->IDR & ROWS){

		row_press = KEYPAD_PORT->IDR & ROWS; // Determine which row was pressed
		KEYPAD_PORT->BRR = COLS; // CLEAR COLUMNS

		//Check if column 0 was pressed, then decode keypad press
		KEYPAD_PORT->BSRR = GPIO_PIN_4; // Turn on COL PIN 0

		if(KEYPAD_PORT->IDR & ROWS){ // Determine if column 0 was pressed
			switch(row_press){
				case 1:
					return 1; // Row 1, Col 0 (1)
					break;
				case(1 << 1):
					return 4; // Row 2, Col 0 (4)
					break;
				case(1 << 2):
					return 7; // Row 3, Col 0 (7)
					break;
				case(1 << 3):
					return 10; // Row 4, Col 0 (* rep. by 10)
					break;
			}
		}

		KEYPAD_PORT->BRR = COLS; // CLEAR COLUMNS

		//Check if column 1 was pressed, then decode keypad press
		KEYPAD_PORT->BSRR = GPIO_PIN_5; // Turn on COL PIN 1

		if(KEYPAD_PORT->IDR & ROWS){ // Determine if column 0 was pressed
			switch(row_press){
				case 1:
					return 2; // Row 1, Col 1 (2)
					break;
				case(1 << 1):
					return 5; // Row 2, Col 1 (5)
					break;
				case(1 << 2):
					return 8; // Row 3, Col 1 (8)
					break;
				case(1 << 3):
					return 0; // Row 4, Col 1 (0)
					break;
			}
		}

		KEYPAD_PORT->BRR = COLS; // CLEAR COLUMNS

		//Check if column 2 was pressed, then decode keypad press
		KEYPAD_PORT->BSRR = GPIO_PIN_6; // Turn on COL PIN 2

		if(KEYPAD_PORT->IDR & ROWS){ // Determine if column 2 was pressed
			switch(row_press){
				case 1:
					return 3; // Row 1, Col 2 (3)
					break;
				case(1 << 1):
					return 6; // Row 2, Col 2 (6)
					break;
				case(1 << 2):
					return 9; // Row 3, Col 2 (9)
					break;
				case(1 << 3):
					return 11; // Row 4, Col 2 (#) (# rep. by 11)
					break;
			}
		}
	}
	return -1; // No button press detected
}
