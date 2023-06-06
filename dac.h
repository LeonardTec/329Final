/*
 * dac.h
 *
 *  Created on: June 6, 2023
 *      Author: Edgar A. Mora
 */

#ifndef INC_DAC_H_
#define INC_DAC_H_
#include "stm32l4xx_hal.h"

// MACRO DEFINITIONS
#define DAC_PORT2 		GPIOB
#define DAC_PORT3 		GPIOC
#define DAC_PORT1 		GPIOE
#define DAC_PORT0		GPIOA

#define DAC_PORT1_EN	RCC_AHB2ENR_GPIOEEN
#define DAC_PORT2_EN	RCC_AHB2ENR_GPIOBEN
#define DAC_PORT3_EN	RCC_AHB2ENR_GPIOCEN
#define DAC_PORT0_EN	RCC_AHB2ENR_GPIOAEN

#define SPI_PORT1_EN    RCC_APB2ENR_SPI1EN
#define SPI_PORT2_EN    RCC_APB1ENR1_SPI2EN
#define SPI_PORT3_EN    RCC_APB1ENR1_SPI3EN

#define DAC_MAX_VOLT	5000
#define DAC_CONTROL		(0x3000)
#define DATA_12BITS     (0x0FFF)
#define GAIN_CALIB		1.01
#define OFFSET_CALIB    4.59

//FUNCTION PROTOTYPES
void DAC_init1(void);
void DAC_init2(void);
void DAC_init3(void);

void SPI_init1(void);
void SPI_init2(void);
void SPI_init3(void);

void DAC_write1(int);
void DAC_write2(int);
void DAC_write3(int);
int  DAC_volt_conv(int);
int  DAC_calibration(int);


#endif /* INC_DAC_H_ */

