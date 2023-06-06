/*
 * dac.c
 *
 *  Created on: May 13, 2023
 *      Author: Edgar A. Mora
 */

#include "dac.h"

/* DAC LAYOUT
			 	 	 _ ____  ____ _
 3.3V	->	VDD		[_| 1  \/  8 |_]   	Vout	<-	Room Lighting
					 _|          |_
  * 	->	!CS   	[_| 2      7 |_]	VSS		<-	GND
              	     _|          |_
 PA5 	->	SCK  	[_| 3      6 |_]	Vref	<-	3.3V
              	     _|          |_
 PA7	->	SDI   	[_| 4      5 |_]    LDAC	<-	GND
					  |__________|
*/

// This function initializes 3 DACs
void DAC_init(void) {
	RCC -> AHB2ENR |= DAC_PORT_EN;				  // CLK enable for DAC_PORT
	RCC -> APB2ENR |= SPI_PORT_EN;                // SPI1 port CLK Enable

	//Pin Mode Configuration: PA5: SCLK, PA7: MOSI
	// PA0: NSS for DAC 1
	// PA1: NSS for DAC 2
	// PA2: NSS for DAC 3

    //Reset pin mode to 0
	DAC_PORT -> MODER &= ~(GPIO_MODER_MODE0 | GPIO_MODER_MODE1 | GPIO_MODER_MODE2 |
						   GPIO_MODER_MODE5 | GPIO_MODER_MODE7);
	// PA0-PA2  (NSS) Set to Output Mode
	DAC_PORT -> MODER |= (GPIO_MODER_MODE0_0 | GPIO_MODER_MODE1_0| GPIO_MODER_MODE2_0);
	// Configure PA5, PA7 for Alternate Function Mode
	DAC_PORT -> MODER |= (GPIO_MODER_MODE5_1 | GPIO_MODER_MODE7_1);
	//Set pins to push-pull
	DAC_PORT -> OTYPER &= ~(GPIO_OTYPER_OT0 | GPIO_OTYPER_OT1 | GPIO_OTYPER_OT2 |
							GPIO_OTYPER_OT5 | GPIO_OTYPER_OT7);
	// Set Registers to No pull up, No pull down Mode [0 0]
	DAC_PORT -> PUPDR  &= ~(GPIO_PUPDR_PUPD0 | GPIO_PUPDR_PUPD1 |GPIO_PUPDR_PUPD2 |
						    GPIO_PUPDR_PUPD5 | GPIO_PUPDR_PUPD7);
	//Sets to High Speed
	DAC_PORT -> OSPEEDR |=  (GPIO_OSPEEDR_OSPEED0 | GPIO_OSPEEDR_OSPEED1 | GPIO_OSPEEDR_OSPEED2 |
							GPIO_OSPEEDR_OSPEED5 | GPIO_OSPEEDR_OSPEED7);
	// Setting Alternate Function
	// Clearing Nibble for Bit 7 AF
	DAC_PORT -> AFR[0]  &= ((0x000F << GPIO_AFRL_AFSEL5_Pos) |
						  	(0x000F << GPIO_AFRL_AFSEL7_Pos));

	DAC_PORT -> AFR[0]  |= ((0x0005 << GPIO_AFRL_AFSEL5_Pos) |
						    (0x0005 << GPIO_AFRL_AFSEL7_Pos));

	// NSS for DACs are currently OFF
	DAC_PORT -> ODR |= (GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2);

	SPI_init(); // Initialize SPI Pins
	DAC_write(0); // Initializes the SPI1 Data Register to ZERO
}


// This function initializes the SPI
void SPI_init( void ) {
	// SPI config as specified @ STM32L4 RM p.1459
	// called by or with DAC_init()
	// build control registers CR1 & CR2 for SPI control of peripheral DAC
	// assumes no active SPI xmits & no recv data in process (BSY=0)
	// CR1 (reset value = 0x0000)
	SPI1->CR1 &= ~( SPI_CR1_SPE );             	// disable SPI for config
	SPI1->CR1 &= ~( SPI_CR1_RXONLY );          	// recv-only OFF
	SPI1->CR1 &= ~( SPI_CR1_LSBFIRST );        	// data bit order MSb:LSb
	SPI1->CR1 &= ~( SPI_CR1_CPOL | SPI_CR1_CPHA ); // SCLK polarity:phase = 0:0
	SPI1->CR1 |=	SPI_CR1_MSTR;              	// MCU is SPI controller

	// Master NSS Pin is NOT Used in Standard Multi-Slave Communication
	// Refer to Page1454 of Reference Manual
	SPI1->CR1 |=    SPI_CR1_SSM;
	SPI1->CR1 |=    SPI_CR1_SSI;

	// CR2 (reset value = 0x0700 : 8b data)
	SPI1->CR2 &= ~( SPI_CR2_TXEIE | SPI_CR2_RXNEIE ); // disable FIFO intrpts
	SPI1->CR2 &= ~( SPI_CR2_FRF);              	// Moto frame format

	// Only used for single pair Master-Slave Communication
	//SPI1->CR2 |=	SPI_CR2_NSSP;              	// auto-generate NSS pulse

	SPI1->CR2 |=	SPI_CR2_DS;                	// 16-bit data
	SPI1->CR2 |=	SPI_CR2_SSOE;              	// enable SS output

	SPI1->CR1 |=	SPI_CR1_SPE;               	// re-enable SPI for ops
}

// This function writes Digital Voltage into the DAC
void DAC_write(int data_in) {
	// Clears the Most Significant Nibble (Control Nibble)
	data_in &= ~(0xF000);
	SPI1 -> DR |= (DAC_CONTROL| data_in);
}

// This function converts Analog Voltage into Digital Voltage for the DAC
int DAC_volt_conv(int voltage) {
	int DAC_value = (DATA_12BITS*voltage)/DAC_MAX_VOLT;
	return DAC_value;
}

// This function calibrates the voltage
int DAC_calibration(int Vout) {
	int new_vout = 0;
	new_vout = (Vout*GAIN_CALIB) + OFFSET_CALIB;
	return new_vout;
}
