/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "lcd.h"
#include "dac.h"
#include "keypad.h"
#include "password.h"
#include "led.h"

void SystemClock_Config(void);

// STATES for FSM
typedef enum  { ST_PASSWORD, ST_CONTROL, ST_SECURITY } state_type;
state_type state = ST_PASSWORD;


int main(void) {
  int attempts = TRIALS;
  // 1: CORRECT, 0: INCORRECT
  int lock = 0;
  //int exit = 0;
  HAL_Init();
  SystemClock_Config();

  // System initializing
  password_init();
  // Initializing LEDS
  LED_control_init();

  // Initializing DAC
  DAC_init();

  // FSM: STATE PASSWORD
  password_welcome();
  for(int i = TRIALS; i > 0; i--) {
	  attempts--;
	  password_update();
	  lock = password_type();
	  password_display(attempts,lock);
	  //password_exit(attempts,lock);
	  if (attempts == 0) {
		  if (!lock) {
			  LCD_init();
			  LCD_home();
			  LCD_write_string("POLICE IS ON ITS");
		  	  LCD_second_line();
		  	  LCD_write_string("WAY, THANK YOU!");
		  	  LED_toggle(2); // Turn OFF Yellow LED
		  	  LED_toggle(3); // Turn On BLUE LED
		  }
	  }
	  if (lock)
		  break;
  }

  // DAC 1 is READY to ACCEPT COMMANDS
  //DAC_PORT -> ODR ^= GPIO_PIN_0;
  // Write 3.3V on DAC1
  DAC_write(0x0FFF);

  // DAC 1 is SENT TO SLEEP
  //DAC_PORT -> ODR ^= GPIO_PIN_0;
  // DAC 2 is READY to ACCEPT COMMANDS
  //DAC_PORT -> ODR ^= GPIO_PIN_1;

  // Write V on DAC2
  //DAC_write(0x08FF);

  while (1) {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void) {

  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void) {
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
