/*
 * alarm.c
 *
 *	Author: Natalie Aynilian
 *
 *	Buzzer Pin: PE1
 *	Blue LED: PF3
 *	Red LED: PF0
 */
#include "alarm.h"

/*
 *	Function: alarm_init(void)
 *	Description:Initialize LED & Buzzer GPIO.
 *				Initialize Timer.
 */
void alarm_init(void){

	/* GPIO Init.*/

	// Enable Clocks
	RCC->AHB2ENR |= LED_PORT_EN;
	RCC->AHB2ENR |= BUZZER_PORT_EN;

	// Set GPIO Mode to Output
	LED_PORT -> MODER   &= ~( GPIO_MODER_MODE3 | GPIO_MODER_MODE0);
	LED_PORT -> MODER   |= GPIO_MODER_MODE3_0 | GPIO_MODER_MODE0_0;

	BUZZER_PORT -> MODER   &= ~( GPIO_MODER_MODE1);
	BUZZER_PORT -> MODER   |= GPIO_MODER_MODE1_0;

	// Push-Pull Reset
	LED_PORT -> OTYPER  &= ~( GPIO_OTYPER_OT3 | GPIO_OTYPER_OT0 );
	BUZZER_PORT -> OTYPER  &= ~( GPIO_OTYPER_OT1);

	// No pull up, pull down
	LED_PORT -> PUPDR  &= ~(  GPIO_PUPDR_PUPD3 | GPIO_PUPDR_PUPD0);
	BUZZER_PORT -> PUPDR &= ~( GPIO_PUPDR_PUPD1 );

	// Medium Speed
	LED_PORT -> OSPEEDR &= ~(GPIO_OSPEEDR_OSPEED3 | GPIO_OSPEEDR_OSPEED0 );
	LED_PORT -> OSPEEDR |=  ( GPIO_OSPEEDR_OSPEED3_0 | GPIO_OSPEEDR_OSPEED0_0 );

	BUZZER_PORT ->  OSPEEDR &= ~( GPIO_OSPEEDR_OSPEED1 );
	BUZZER_PORT ->  OSPEEDR |= ( GPIO_OSPEEDR_OSPEED1 );

	// Start with one LED on
	LED_PORT -> ODR |= (GPIO_PIN_3);
	BUZZER_PORT -> ODR |= GPIO_PIN_1;

	/* Timer 2 Initialization */
	 RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;           // enable clock for TIM2
	 TIM2->DIER |= (TIM_DIER_CC1IE | TIM_DIER_UIE);  // enable event gen, rcv CCR1
	 TIM2->ARR = 0xFFFFFFFF;
	 TIM2-> PSC = 10000;
	 TIM2->CCR1 = 650000;
	 TIM2->SR &= ~(TIM_SR_CC1IF | TIM_SR_UIF);       // clr IRQ flag in status reg
	 NVIC->ISER[0] |= (1 << (TIM2_IRQn & 0x1F));     // set NVIC interrupt: 0x1F
	 __enable_irq();                                 // global IRQ enable
	 TIM2->CR1 |= TIM_CR1_CEN;                       // start TIM2 CR1

}

/*
 * Function: TIM2_IRQHandler(void)
 * Description: Timer 2 ISR
 */
void TIM2_IRQHandler(void){
	if (TIM2->SR & TIM_SR_CC1IF) {			// triggered by CCR1 event

		// Toggle Buzzer and LEDs for Alarm affect
		BUZZER_PORT-> ODR ^= (GPIO_PIN_1);
		LED_PORT -> ODR ^= (GPIO_PIN_3);
		LED_PORT -> ODR ^= (GPIO_PIN_0);

		TIM2 -> CNT = 0;

		TIM2->SR &= ~(TIM_SR_CC1IF);		// manage the flag
	}

	if (TIM2->SR & TIM_SR_UIF) {        // triggered by ARR event
		TIM2->SR &= ~(TIM_SR_UIF);       // manage the flag
	}
}




