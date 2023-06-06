/*
 * lcd.c
 *
 *  Created on: May 13, 2023
 *      Author: Edgar A. Mora
 */

/* Includes ------------------------------------------------------------------*/
#include "lcd.h"
#include <string.h>

/*LCD PINOUT -------------------------------------------------------------------------------------------------
	| VSS | VDD | DNC | RS  | R/W  |  E  | DB0 | DB1 | DB2 | DB3 | DB4 | DB5 | DB6 | DB7 | LED+ | LED- |

  NUCLEO L496ZG Pin Connections
	| GND |3.3V | 	  | PB4 | PB5  | PB6 |     |     |     |     | PB0 | PB1 | PB2 | PB3 | 3.3V |  GND |  */

/* FUNCTIONS-------------------------------------------------------------------*/
/**
 * This function initializes the GPIO Pins for LCD
 *
 * @param: None
 * @return: N/A
 */
void LCD_GPIO_init(void) {
	// LCD_PORT BITS:
	// Bits 4-7: output data nibble.
	// Bits 0-2 : RS,R/W,EN Control Signals Respectively
	// Enable Clock for GPIO Port B
	RCC->AHB2ENR |= LCD_PORT_EN;

	// Clearing Contents of PB0-PB2, PB4-PB7 MODER Register
	LCD_PORT->MODER   &= ~( GPIO_MODER_MODE0 |
	                        GPIO_MODER_MODE1 |
	                        GPIO_MODER_MODE2 |
	                        GPIO_MODER_MODE3 |

							GPIO_MODER_MODE4 |
							GPIO_MODER_MODE5 |
							GPIO_MODER_MODE6 );

	//Setting PB0-PB2, PB4-PB7 to General Purpose Output Mode [0 1]
	LCD_PORT->MODER  |=  ( GPIO_MODER_MODE0_0 |
			               GPIO_MODER_MODE1_0 |
						   GPIO_MODER_MODE2_0 |
			               GPIO_MODER_MODE3_0 |

						   GPIO_MODER_MODE4_0 |
						   GPIO_MODER_MODE5_0 |
						   GPIO_MODER_MODE6_0 );

	// Clearing Contents of PB0-PB2, PB4-PB7 OTYPER Register
	// This also sets them all to push/pull Mode [0]
	LCD_PORT->OTYPER  &= ~( GPIO_OTYPER_OT0 |
            				GPIO_OTYPER_OT1 |
							GPIO_OTYPER_OT2 |
							GPIO_OTYPER_OT3 |

							GPIO_OTYPER_OT4 |
							GPIO_OTYPER_OT5 |
							GPIO_OTYPER_OT6 );

	// Clearing Contents of PB0-PB2, PB4-PB7 PUPDR Register
	// This also sets them all to No pull up, No pull down Mode [0 0]
	LCD_PORT->PUPDR  &= ~( GPIO_PUPDR_PUPD0 |
            			   GPIO_PUPDR_PUPD1 |
						   GPIO_PUPDR_PUPD2 |
					       GPIO_PUPDR_PUPD3 |

						   GPIO_PUPDR_PUPD4 |
						   GPIO_PUPDR_PUPD5 |
						   GPIO_PUPDR_PUPD6 );

	// Clearing contents of PB0-PB2, PB4-PB7 OSPEEDR Register
    LCD_PORT->OSPEEDR &= ~( GPIO_OSPEEDR_OSPEED0 |
	  		  	  	  	  	GPIO_OSPEEDR_OSPEED1 |
	  					    GPIO_OSPEEDR_OSPEED2 |
	    					GPIO_OSPEEDR_OSPEED3 |

	    	  		  	  	GPIO_OSPEEDR_OSPEED4 |
	    	  			    GPIO_OSPEEDR_OSPEED5 |
							GPIO_OSPEEDR_OSPEED6 );

    // Setting PB0-PB2, PB4-PB7 to Highest Speed Mode [1 1]
	LCD_PORT->OSPEEDR |=  ( GPIO_OSPEEDR_OSPEED0_0 | GPIO_OSPEEDR_OSPEED0_1 |
	    				    GPIO_OSPEEDR_OSPEED1_0 | GPIO_OSPEEDR_OSPEED1_1 |
							GPIO_OSPEEDR_OSPEED2_0 | GPIO_OSPEEDR_OSPEED2_1 |
							GPIO_OSPEEDR_OSPEED3_0 | GPIO_OSPEEDR_OSPEED3_1 |

						    GPIO_OSPEEDR_OSPEED4_0 | GPIO_OSPEEDR_OSPEED4_1 |
						    GPIO_OSPEEDR_OSPEED5_0 | GPIO_OSPEEDR_OSPEED5_1 |
							GPIO_OSPEEDR_OSPEED6_0 | GPIO_OSPEEDR_OSPEED6_1 );
}

/**
 * This function clears the contents of the LCD screen
 *
 * @arguments: None
 * @return: N/A
 */
void LCD_clear() {
    LCD_command(CLR_DISP);
    delay_us( 1600 );
}

/**
 * This function clears the screen and returns cursor to its original position
 *
 * @arguments: None
 * @return: N/A
 */
void LCD_home() {
    LCD_command(GO_HOME);
	delay_us( 1600 );
}

/**
 * This function moves cursor to the second line
 *
 * @arguments: None
 * @return: N/A
 */
void LCD_second_line(void) {
	LCD_command(SECOND_LINE);
	delay_us( 40 );
}

/**
 * This function writes a string of characters
 *
 * @arguments: Pointer to a string of characters
 * @return: N/A
 * @note: Only works properly for 16-characters per string max
 */
void LCD_write_string(char *string) {
	// Iterates through each character in the string and writes it on the LCD screen
	for( int i=0; i < strlen(string);i++) {
		LCD_write_char(string[i]);
	}
}

/**
 * This function writes a single number as a character
 *
 * @arguments: Number to be written as character
 * @return: N/A
 * @note: This function can only display single digit numbers 0-9
 */
void LCD_write_number(int number) {
	char num = number + '0';
	LCD_write_char(num);
}

/* -----------------------GIVEN CODE IN A3--------------------------------------*/
/**
 * This function initializes the LCD registers
 *
 * @arguments: None
 * @return: N/A
 */
void LCD_init( void ) {
	// Function Set 1
	delay_us( 40000 ); // power-up wait 40 ms
	for ( int idx = 0; idx < 3; idx++ ) { // wake up 1,2,3: DATA = 0011 XXXX
		LCD_4b_command( 0x30 ); // HI 4b of 8b cmd, low nibble = X
		delay_us( 200 );
	}
	// Function Set 4
	LCD_4b_command( 0x20 );
	delay_us( 40 );
	// Function Set 5
	LCD_command( 0x28 ); // 4b/2-line: DATA = 0010 10XX
	delay_us( 40 );
	// Display ON/OFF Control
	LCD_command( 0x0F );	// Display on, Cursor on, Blinking on; DATA = 0000 1111
	delay_us(40);
	// DIsplay Clear
	LCD_command( 0x01 ); // DATA = 0000 0001
	delay_us( 1520 );  // 1.52ms Delay
	// Entry Mode
	LCD_command( 0x06 ); // Increment by 1, No Shift; DATA = 0000 0110
	delay_us( 40 );
}

/**
 * This function toggles the Enable Register
 *
 * @arguments: None
 * @return: N/A
 */
void LCD_pulse_ENA( void ) {
	// ENAble line sends command on falling edge
	// set to restore default then clear to trigger
	LCD_PORT->ODR |= ( LCD_EN ); // ENABLE = HI
	delay_us( 5 ); // TDDR > 320 ns
	LCD_PORT->ODR &= ~( LCD_EN ); // ENABLE = LOW
	delay_us( 5 );
}

/**
 * This function sends commands using only the high nibble
 *
 * @arguments: 8-bits command
 * @return: N/A
 */
void LCD_4b_command( uint8_t command ) {
	// LCD command using high nibble only - used for 'wake-up' 0x30 commands
	LCD_PORT->ODR &= ~( LCD_DATA_BITS ); // clear DATA bits
	LCD_PORT->ODR |= ( command >> 4 ); // DATA = command
	delay_us( 5 );
	LCD_pulse_ENA( );
}

/**
 * This function sends commands to the LCD (high and low nibble)
 *
 * @arguments: 8-bits command
 * @return: N/A
 */
void LCD_command( uint8_t  command ) {
	// send command to LCD in 4-bit instruction mode
	// HIGH nibble then LOW nibble, timing sensitive
	LCD_PORT->ODR &= ~( LCD_DATA_BITS ); // isolate cmd bits
	LCD_PORT->ODR |= ( (command>>4) & LCD_DATA_BITS ); // HIGH shifted low
	delay_us( 5 );
	LCD_pulse_ENA( ); // latch HIGH NIBBLE

	LCD_PORT->ODR &= ~( LCD_DATA_BITS ); // isolate cmd bits
	LCD_PORT->ODR |= ( command & LCD_DATA_BITS ); // LOW nibble
	delay_us( 5 );
	LCD_pulse_ENA( ); // latch LOW NIBBLE
}

/**
 * This function writes a single character on the LCD screen
 *
 * @arguments: 8-bits letter
 * @return: N/A
 */
void LCD_write_char( uint8_t  letter ) {
	// calls LCD_command() w/char data; assumes all ctrl bits set LO in LCD_init()
	LCD_PORT->ODR |= (LCD_RS); // RS = HI for data to address
	delay_us( 5 );
	LCD_command( letter ); // character to print
	LCD_PORT->ODR &= ~(LCD_RS); // RS = LO
}

/**
 * This function configures SysTick timer for use with delay_us().
 *
 * @arguments: None
 * @return: N/A
 * @note: breaks HAL_delay() by disabling interrupts for shorter delay timing.
 */
void SysTick_Init(void) {
	SysTick->CTRL |= (SysTick_CTRL_ENABLE_Msk | // enable SysTick Timer
			SysTick_CTRL_CLKSOURCE_Msk); // select CPU clock
	SysTick->CTRL &= ~(SysTick_CTRL_TICKINT_Msk); // disable interrupt
}

/**
 * This function delays in microseconds using SysTick timer to count CPU clock cycles,
 * do not call with 0 : error, maximum delay.
 * Careful calling with small nums : results in longer delays than specified:
 * e.g. @4MHz, delay_us(1) = 10=15 us delay.
 *
 * @arguments: 32bits delay time
 * @return: N/A
 */
void delay_us(const uint32_t  time_us) {
	// set the counts for the specified delay
	SysTick->LOAD = (uint32_t)((time_us * (SystemCoreClock / 1000000)) - 1);
	SysTick->VAL = 0; // clear timer count
	SysTick->CTRL &= ~(SysTick_CTRL_COUNTFLAG_Msk); // clear count flag
	while (!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk)); // wait for flag
}
