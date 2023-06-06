/*
 * dac.c
 *
 *  Created on: June 6, 2023
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
void DAC_init1(void) {
	RCC -> AHB2ENR |= DAC_PORT1_EN;				// CLK Enable for GPIOE
	RCC -> APB2ENR |= SPI_PORT1_EN;             // SPI1 port CLK Enable

    //Reset pin mode to 0
	DAC_PORT1 -> MODER &= ~(GPIO_MODER_MODE12 | GPIO_MODER_MODE13| GPIO_MODER_MODE15);
	//DAC_PORT0 -> MODER &= ~(GPIO_MODER_MODE12);
	// Configure PE12, PA5, PA7 for Alternate Function Mode
	DAC_PORT1 -> MODER |= (GPIO_MODER_MODE12_1 | GPIO_MODER_MODE13_1 | GPIO_MODER_MODE15_1);
	//DAC_PORT0 -> MODER |= (GPIO_MODER_MODE12_1);
	//Set pins to push-pull
	DAC_PORT1 -> OTYPER &= ~(GPIO_OTYPER_OT12 | GPIO_OTYPER_OT13 | GPIO_OTYPER_OT15);
	//DAC_PORT0 -> OTYPER &= ~(GPIO_OTYPER_OT12);

	// Set Registers to No pull up, No pull down Mode [0 0]
	DAC_PORT1 -> PUPDR  &= ~(GPIO_PUPDR_PUPD12 | GPIO_PUPDR_PUPD13 | GPIO_PUPDR_PUPD15);
	//DAC_PORT0 -> PUPDR  &= ~(GPIO_PUPDR_PUPD12);
	//Sets to High Speed
	DAC_PORT1 -> OSPEEDR |= (GPIO_OSPEEDR_OSPEED12 | GPIO_OSPEEDR_OSPEED13 | GPIO_OSPEEDR_OSPEED15);
	//DAC_PORT0 -> OSPEEDR |= (GPIO_OSPEEDR_OSPEED12);
	// Setting Alternate Function
	// Clearing Nibble for Bit 7 AF
	DAC_PORT1 -> AFR[1]  &= ((0x000F << GPIO_AFRH_AFSEL12_Pos) | (0x000F << GPIO_AFRH_AFSEL13_Pos) |
							(0x000F << GPIO_AFRH_AFSEL15_Pos));
	//DAC_PORT0 -> AFR[1]  &= (0x000F << GPIO_AFRH_AFSEL12_Pos);
	// Selecting Alternative Function 5
	DAC_PORT1 -> AFR[1]  |= ((0x0005 << GPIO_AFRH_AFSEL12_Pos) | (0x0005 << GPIO_AFRH_AFSEL13_Pos) |
								(0x0005 << GPIO_AFRH_AFSEL15_Pos));

	SPI_init1();   // Initialize SPI Pins
	DAC_write1(0); // Initializes the SPI1 Data Register to ZERO

}

void DAC_init2(void) {
	RCC -> AHB2ENR |= DAC_PORT2_EN;				   // CLK enable for DAC_PORT
	RCC -> APB1ENR1 |= SPI_PORT2_EN;                // SPI1 port CLK Enable

    //Reset pin mode to 0
	DAC_PORT2 -> MODER &= ~(GPIO_MODER_MODE9| GPIO_MODER_MODE10 | GPIO_MODER_MODE15);
	// Configure Pa4, PA5, PA7 for Alternate Function Mode
	DAC_PORT2 -> MODER |= (GPIO_MODER_MODE9_1| GPIO_MODER_MODE10_1 | GPIO_MODER_MODE15_1);
	//Set pins to push-pull
	DAC_PORT2 -> OTYPER &= ~(GPIO_OTYPER_OT9 | GPIO_OTYPER_OT10 | GPIO_OTYPER_OT15);
	// Set Registers to No pull up, No pull down Mode [0 0]
	DAC_PORT2 -> PUPDR  &= ~(GPIO_PUPDR_PUPD9|GPIO_PUPDR_PUPD10 | GPIO_PUPDR_PUPD15);
	//Sets to High Speed
	DAC_PORT2 -> OSPEEDR |= (GPIO_OSPEEDR_OSPEED9|GPIO_OSPEEDR_OSPEED10 | GPIO_OSPEEDR_OSPEED15);
	// Setting Alternate Function
	// Clearing Nibble for Bit 7 AF
	DAC_PORT2 -> AFR[1]  &= ~((0x000F << GPIO_AFRH_AFSEL9_Pos) | (0x000F << GPIO_AFRH_AFSEL10_Pos) |
						  	(0x000F << GPIO_AFRH_AFSEL15_Pos));

	DAC_PORT2 -> AFR[1]  |= ((0x0005 << GPIO_AFRH_AFSEL9_Pos) | (0x0005 << GPIO_AFRH_AFSEL10_Pos) |
						    (0x0005 << GPIO_AFRH_AFSEL15_Pos));


	SPI_init2(); // Initialize SPI Pins
	DAC_write2(0); // Initializes the SPI2 Data Register to ZERO

}

void DAC_init3(void) {
	RCC -> AHB2ENR  |= DAC_PORT3_EN;			   // CLK enable for DAC_PORT
	RCC -> AHB2ENR  |= DAC_PORT0_EN;
	RCC -> APB1ENR1 |= SPI_PORT3_EN;               // SPI3 port CLK Enable

    //Reset pin mode to 0
	DAC_PORT3 -> MODER &= ~(GPIO_MODER_MODE10 | GPIO_MODER_MODE12);
	DAC_PORT0 -> MODER &= ~(GPIO_MODER_MODE4);
	// Configure PC10, PC12, PA15 for Alternate Function Mode
	DAC_PORT3 -> MODER |= (GPIO_MODER_MODE10_1| GPIO_MODER_MODE12_1);
	DAC_PORT0 -> MODER |= (GPIO_MODER_MODE4_1);
	//Set pins to push-pull
	DAC_PORT3 -> OTYPER &= ~(GPIO_OTYPER_OT10 | GPIO_OTYPER_OT12);
	DAC_PORT0-> OTYPER &= ~(GPIO_OTYPER_OT4);
	// Set Registers to No pull up, No pull down Mode [0 0]
	DAC_PORT3 -> PUPDR  &= ~(GPIO_PUPDR_PUPD10 | GPIO_PUPDR_PUPD12);
	DAC_PORT0 -> PUPDR  &= ~(GPIO_PUPDR_PUPD4);
	//Sets to High Speed
	DAC_PORT3 -> OSPEEDR |= (GPIO_OSPEEDR_OSPEED10 | GPIO_OSPEEDR_OSPEED12);
	DAC_PORT0 -> OSPEEDR |= (GPIO_OSPEEDR_OSPEED4);

	// Setting Alternate Function
	// Clearing Nibble for Bit 7 AF
	DAC_PORT3 -> AFR[1]  &= ((0x000F << GPIO_AFRH_AFSEL10_Pos) | (0x000F << GPIO_AFRH_AFSEL12_Pos));
	DAC_PORT0 -> AFR[0]  &= (0x000F << GPIO_AFRL_AFSEL4_Pos);

	DAC_PORT3 -> AFR[1]  |= ((0x0006 << GPIO_AFRH_AFSEL10_Pos) | (0x0006 << GPIO_AFRH_AFSEL12_Pos));
	DAC_PORT0 -> AFR[0]  |= (0x0006 << GPIO_AFRL_AFSEL4_Pos);

	SPI_init3(); // Initialize SPI Pins
	DAC_write3(0); // Initializes the SPI1 Data Register to ZERO

}

// This function initializes the SPI
void SPI_init1( void ) {
	// SPI config as specified @ STM32L4 RM p.1459
	// called by or with DAC_init()
	// build control registers CR1 & CR2 for SPI control of peripheral DAC
	// assumes no active SPI xmits & no recv data in process (BSY=0)
	// CR1 (reset value = 0x0000)
	SPI1->CR1 &= ~( SPI_CR1_SPE );             	// disable SPI for config
	SPI1->CR1 &= ~( SPI_CR1_RXONLY );          	// recv-only OFF / Transmit-Only ON
	SPI1->CR1 &= ~( SPI_CR1_LSBFIRST );        	// data bit order MSb:LSb
	SPI1->CR1 &= ~( SPI_CR1_CPOL | SPI_CR1_CPHA ); // SCLK polarity:phase = 0:0
	SPI1->CR1 |=	SPI_CR1_MSTR;              	// MCU is SPI controller

	// CR2 (reset value = 0x0700 : 8b data)
	SPI1->CR2 &= ~( SPI_CR2_TXEIE | SPI_CR2_RXNEIE ); // disable FIFO intrpts
	SPI1->CR2 &= ~( SPI_CR2_FRF);              	// Moto frame format

	// Only used for single pair Master-Slave Communication
	SPI1->CR2 |=	SPI_CR2_NSSP;              	// auto-generate NSS pulse

	SPI1->CR2 |=	SPI_CR2_DS;                	// 16-bit data
	SPI1->CR2 |=	SPI_CR2_SSOE;              	// enable SS output

	SPI1->CR1 |=	SPI_CR1_SPE;               	// re-enable SPI for ops
}

// This function initializes the SPI
void SPI_init2( void ) {
	// SPI config as specified @ STM32L4 RM p.1459
	// called by or with DAC_init()
	// build control registers CR1 & CR2 for SPI control of peripheral DAC
	// assumes no active SPI xmits & no recv data in process (BSY=0)
	// CR1 (reset value = 0x0000)
	SPI2->CR1 &= ~( SPI_CR1_SPE );             	// disable SPI for config
	SPI2->CR1 &= ~( SPI_CR1_RXONLY );          	// recv-only OFF / Transmit-Only ON
	SPI2->CR1 &= ~( SPI_CR1_LSBFIRST );        	// data bit order MSb:LSb
	SPI2->CR1 &= ~( SPI_CR1_CPOL | SPI_CR1_CPHA ); // SCLK polarity:phase = 0:0
	SPI2->CR1 |=	SPI_CR1_MSTR;              	// MCU is SPI controller

	// CR2 (reset value = 0x0700 : 8b data)
	SPI2->CR2 &= ~( SPI_CR2_TXEIE | SPI_CR2_RXNEIE ); // disable FIFO intrpts
	SPI2->CR2 &= ~( SPI_CR2_FRF);              	// Moto frame format

	// Only used for single pair Master-Slave Communication
	SPI2->CR2 |=	SPI_CR2_NSSP;              	// auto-generate NSS pulse

	SPI2->CR2 |=	SPI_CR2_DS;                	// 16-bit data
	SPI2->CR2 |=	SPI_CR2_SSOE;              	// enable SS output

	SPI2->CR1 |=	SPI_CR1_SPE;               	// re-enable SPI for ops
}

// This function initializes the SPI
void SPI_init3( void ) {
	// SPI config as specified @ STM32L4 RM p.1459
	// called by or with DAC_init()
	// build control registers CR1 & CR2 for SPI control of peripheral DAC
	// assumes no active SPI xmits & no recv data in process (BSY=0)
	// CR1 (reset value = 0x0000)
	SPI3->CR1 &= ~( SPI_CR1_SPE );             	// disable SPI for config
	SPI3->CR1 &= ~( SPI_CR1_RXONLY );          	// recv-only OFF / Transmit-Only ON
	SPI3->CR1 &= ~( SPI_CR1_LSBFIRST );        	// data bit order MSb:LSb
	SPI3->CR1 &= ~( SPI_CR1_CPOL | SPI_CR1_CPHA ); // SCLK polarity:phase = 0:0
	SPI3->CR1 |=	SPI_CR1_MSTR;              	// MCU is SPI controller

	// CR2 (reset value = 0x0700 : 8b data)
	SPI3->CR2 &= ~( SPI_CR2_TXEIE | SPI_CR2_RXNEIE ); // disable FIFO intrpts
	SPI3->CR2 &= ~( SPI_CR2_FRF);              	// Moto frame format

	// Only used for single pair Master-Slave Communication
	SPI3->CR2 |=	SPI_CR2_NSSP;              	// auto-generate NSS pulse

	SPI3->CR2 |=	SPI_CR2_DS;                	// 16-bit data
	SPI3->CR2 |=	SPI_CR2_SSOE;              	// enable SS output

	SPI3->CR1 |=	SPI_CR1_SPE;               	// re-enable SPI for ops
}

// This function writes Digital Voltage into the DAC
void DAC_write1(int data_in) {
	// Clears the Most Significant Nibble (Control Nibble)
	data_in &= ~(0xF000);
	SPI1 -> DR |= (DAC_CONTROL| data_in);
}
// This function writes Digital Voltage into the DAC
void DAC_write2(int data_in) {
	// Clears the Most Significant Nibble (Control Nibble)
	data_in &= ~(0xF000);
	SPI2 -> DR |= (DAC_CONTROL| data_in);
}
// This function writes Digital Voltage into the DAC
void DAC_write3(int data_in) {
	// Clears the Most Significant Nibble (Control Nibble)
	data_in &= ~(0xF000);
	SPI3 -> DR |= (DAC_CONTROL| data_in);
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
