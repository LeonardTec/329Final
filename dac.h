/*
 * dac.h
 *
 *  Created on: May 30, 2023
 *      Author: Edgar A. Mora
 */

#ifndef INC_DAC_H_
#define INC_DAC_H_
#include "stm32l4xx_hal.h"

// MACRO DEFINITIONS
#define DAC_PORT 		GPIOA
#define DAC_PORT_EN		RCC_AHB2ENR_GPIOAEN
#define SPI_PORT_EN     RCC_APB2ENR_SPI1EN
#define DAC_MAX_VOLT	3300
#define DAC_CONTROL		(0x3000)
#define DATA_12BITS     (0x0FFF)
#define GAIN_CALIB		1.01
#define OFFSET_CALIB    4.59

//FUNCTION PROTOTYPES
void DAC_init(void);
void SPI_init(void);
void DAC_write(int);
int  DAC_volt_conv(int);
int  DAC_calibration(int);


#endif /* INC_DAC_H_ */
