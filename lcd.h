/*
 * lcd.h
 *
 *  Created on: May 30, 2023
 *      Author: Edgar A. Mora
 */

#ifndef INC_LCD_H_
#define INC_LCD_H_
#include "stm32l4xx_hal.h"

/* Function prototypes----------------------------------------------------*/
void LCD_init(void);
void LCD_GPIO_init(void);
void LCD_pulse_ENA(void);
void LCD_write_string(char *text);
void LCD_4b_command(uint8_t  command);
void LCD_command(uint8_t  command);
void LCD_write_char(uint8_t letter);
void LCD_write_number(int number);
void LCD_clear(void);
void LCD_home(void);
void LCD_second_line(void);
void SysTick_Init(void);
void delay_us(const uint32_t time_us);

/* MACRO Definitions-------------------------------------------------------*/
#define LCD_PORT		GPIOB
#define LCD_PORT_EN		RCC_AHB2ENR_GPIOBEN
#define LCD_DATA_BITS	0x0F

// Control Signals for LCD
#define LCD_RS 			0x10
#define LCD_RW 			0x20
#define LCD_EN 			0x40

// Commands
#define CLR_DISP        0x01
#define GO_HOME         0x02
#define SECOND_LINE		0xC0


#endif /* INC_LCD_H_ */
