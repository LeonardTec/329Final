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
#include "alarm.h"
#include "control.h"

void SystemClock_Config(void);

// STATES for FSM
typedef enum  { ST_PASSWORD, ST_CONTROL, ST_SECURITY, ST_ROOM1, ST_ROOM2, ST_ROOM3,
				ST_FAN, ST_LIGHT1, ST_BRIGHT1, ST_BRIGHT2, ST_BRIGHT3} state_type;
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

  // Initializing DACs
  DAC_init1();
  DAC_init2();
  DAC_init3();

  // Variables for Room Brightness, default to minimum
  int light1 = MIN_BRIGHTNESS;
  int light2 = MIN_BRIGHTNESS;
  int light3 = MIN_BRIGHTNESS;

  while (1) {
	  switch(state) {
	  case ST_PASSWORD: 	  // FSM: STATE PASSWORD
	  	  password_welcome(); // Ask user for 5-digit password
	  	  // 5 Attempts to get correct password
	      for(int i = TRIALS; i > 0; i--) {
	    	  attempts--;			// 1 Attempt less
	    	  password_update();	// Prompts for input
	    	  lock = password_type();	// Returns a flag of Password was Correct [1] or not [0]
	    	  // Was the password correct?
	    	  if (lock) {	// Yes
	    		  password_display_correct();
	    		  state = ST_CONTROL;	// Change to CONTROL STATE
	    		  break;				//  Exit the for-loop
	    	  }
	    	  // Incorrect Password
	    	  else {
	    		  password_display_incorrect(attempts);	// Display incorrect password
	    		  if (attempts == 0) { // If you ran out attempts
	    			  password_display_incorrect_security(); // Display for police
	    			  state = ST_SECURITY;	// Enter SECURITY STATE
	    			  break;	// Break from for-loop
	    		  }

	    	  } // End of else-statement
	    } // End of for-loop
	      break;

	  case ST_SECURITY:
		  alarm_init();
		  while(1){}	// Stuck here foreva!
		  break;

	  case ST_CONTROL: // STATE CONTROL for FSM
		  control_room_select();
		  int room = 0;
		  room = control_keypad();
		  if (room == 1)
			  state = ST_ROOM1;
		  else if (room == 2)
			  state = ST_ROOM2;
		  else if (room == 3)
			  state = ST_ROOM3;
		  break;

	 case ST_ROOM1:
		 control_room1_display();
		 //int command = -1;
		 int command1 = control_keypad();
		 if (command1 == 1)
			 state = ST_LIGHT1;
		 else if (command1 == 2)
			 state = ST_FAN;
		 else if (command1 == 9)
			 state = ST_CONTROL;
		 break;

	 case ST_ROOM2:
		 control_light2or3();
		 //int command = -1;
		 int command2 = control_keypad();
		 if (command2 == 1)
			 state = ST_BRIGHT2;
		 else if (command2 == 2)
			 DAC_write2(0);
		 else if (command2 == 0)
			 state = ST_CONTROL;
		 break;

	 case ST_ROOM3:
	 	 control_light2or3();
	 	 //int command = -1;
		 int command3 = control_keypad();
	 	 if (command3 == 1)
	 		 state = ST_BRIGHT3;
		 else if (command3 == 2)
			 DAC_write3(0);
		 else if (command3 == 0)
 			 state = ST_CONTROL;
		 break;

	 case ST_LIGHT1:
	 	 control_light1();
	 	 //int command = -1;
		 int command4 = control_keypad();
	 	 if (command4 == 1)
	 		 state = ST_BRIGHT1;
		 else if (command4 == 2)
			 DAC_write1(0);
		 else if (command4 == 0)
 			 state = ST_ROOM1;
		 else if (command4 == 9)
			 state = ST_CONTROL;
		 break;

	 case ST_BRIGHT1:
		 DAC_write1(light1);
		 control_brightness();
		 //int key = -1;
		 int key1 = control_keypad();
		 if (key1 == 1) {
			 if (light1 < 4000) {
				 light1 += 200;
				 DAC_write1(light1);
			 }
		 }
		 else if (key1 == 2) {
			 if (light1 > 2200) {
				 light1 -= 200;
				 DAC_write1(light1);
			 }
		 }
		 else if (key1 == 0)
			 state = ST_LIGHT1;
		 else if (key1 == 9)
			 state = ST_CONTROL;
		 break;

	 case ST_BRIGHT2:
		 DAC_write2(light2);
		 control_brightness();
		 //int key = -1;
		 int key2 = control_keypad();
		 if (key2 == 1) {
			 if (light2 < 4000) {
				 light2 += 200;
				 DAC_write2(light2);
			 }
		 }
		 else if (key2 == 2) {
			 if (light2 > 2200) {
				 light2 -= 200;
				 DAC_write2(light2);
			 }
		 }
		 else if (key2 == 0)
			 state = ST_ROOM2;
		 else if (key2 == 9)
			 state = ST_CONTROL;
		 break;

	 case ST_BRIGHT3:
		 DAC_write3(light3);
		 control_brightness();
		 //int key = -1;
		 int key3 = control_keypad();
		 if (key3 == 1) {
			 if (light3 < 4000) {
				 light3 += 200;
				 DAC_write3(light3);
			 }
		 }
		 else if (key3 == 2) {
			 if (light3 > 2200) {
				 light3 -= 200;
				 DAC_write3(light3);
			 }
		 }
		 else if (key3 == 0)
			 state = ST_ROOM3;
		 else if (key3 == 9)
			 state = ST_CONTROL;
		 break;

	 case ST_FAN:
		 control_fan_init();
		 control_fan();
		 //int key = -1;
		 int key4 = control_keypad();
		 if (key4 == 1)
			 MOTOR_PORT -> ODR ^= GPIO_PIN_0;
		 else if (key4 == 0)
			 state = ST_ROOM1;
		 else if (key4 == 9)
			 state = ST_CONTROL;
		 break;
 	  }// End of switch-statement
  } // End of while-loop
} // End of Main

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
