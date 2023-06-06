/*
 * alarm.h
 *
 */
#ifndef INC_ALARM_H_
#define INC_ALARM_H_

#include "stm32l4xx_hal.h"

// Pre-Defined GPIO PORTS
#define LED_PORT GPIOF
#define LED_PORT_EN RCC_AHB2ENR_GPIOEFN
#define BUZZER_PORT GPIOE
#define BUZZER_PORT_EN RCC_AHB2ENR_GPIOEEN

// Function Prototypes
void alarm_init(void);
void TIM2_IRQHandler(void);

#endif /* INC_ALARM_H_ */
