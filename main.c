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
  int lock = 0;   // 1: CORRECT, 0: INCORRECT
  HAL_Init();
  SystemClock_Config();

  // Initializing System
  password_init();

  // Initializing State LEDs
  LED_control_init();
	// Initialize the FAN GPIO PIN
  control_fan_init();
  // Initializing DACs
  DAC_init1();
  DAC_init2();
  DAC_init3();

  // Variables for Room Brightness, default to 50% intensity (3000mV)
  int light1 = DEFAULT_BRIGHTNESS;
  int light2 = DEFAULT_BRIGHTNESS;
  int light3 = DEFAULT_BRIGHTNESS;

  // Begin of while loop
  while (1) {
	  switch(state) {
	  case ST_PASSWORD: 	  // FSM: STATE PASSWORD
	  	  password_welcome(); // Ask user for 5-digit password
	      for(int i = TRIALS; i > 0; i--) {  // 5 Attempts to get correct password
	    	  attempts--;			// 1 Attempt less
	    	  password_update();	// Prompts for input
	    	  lock = password_type();	// Returns a flag of Password was Correct [1] or not [0]
	    	  // Was the password correct?
	    	  if (lock) {	// Yes
	    		  password_display_correct(); // Display Correct password
	    		  state = ST_CONTROL;	// Change to CONTROL STATE
	    		  break;				//  Exit the for-loop
	    	  }
	    	  else { 	    	  // Incorrect Password
	    		  password_display_incorrect(attempts);	// Display incorrect password
	    		  if (attempts == 0) { // If you ran out attempts
	    			  password_display_incorrect_security(); // Display Security Message
	    			  state = ST_SECURITY;	// Enter SECURITY STATE
	    			  break;	// Break from for-loop
	    		  }
	    	  } // End of else-statement
	    } // End of for-loop
	      break;

	  case ST_SECURITY:		// STATE SECURITY
		  alarm_init();		// Trigger Alarm
		  while(1){}		// Stay here until hard-reset
		  break;

	  case ST_CONTROL: // STATE CONTROL
		  control_room_select();	// Prompt which room to select
		  int room = control_keypad();	// Get value from keypad
		  if (room == 1)			// Room 1
			  state = ST_ROOM1;
		  else if (room == 2)		// Room 2
			  state = ST_ROOM2;
		  else if (room == 3)		// Room 3
			  state = ST_ROOM3;
		  break;					// Invalid keypress

	 case ST_ROOM1:	// ROOM 1 STATE
		 control_room1_display();	// Prompt for Light or Fan
		 int command1 = control_keypad();	// Get value from keypad
		 if (command1 == 1)			// Lights
			 state = ST_LIGHT1;
		 else if (command1 == 2)	// Fan
			 state = ST_FAN;
		 else if (command1 == 9)	// Return
			 state = ST_CONTROL;
		 break;

	 case ST_ROOM2:	// ROOM 2 STATE
		 control_light2or3();		// Prompt for Lights ON or OFF
		 int command2 = control_keypad(); // Get value from keypad
		 if (command2 == 1)			// Lights ON
			 state = ST_BRIGHT2;
		 else if (command2 == 2)	// Lights OFF
			 DAC_write2(0);
		 else if (command2 == 0)	// Return
			 state = ST_CONTROL;
		 break;

	 case ST_ROOM3: // ROOM 3 STATE
	 	 control_light2or3();		// Prompt for Lights ON or OFF
		 int command3 = control_keypad(); // Get value from keypad
	 	 if (command3 == 1)			// Lights ON
	 		 state = ST_BRIGHT3;
		 else if (command3 == 2)	// Lights OFF
			 DAC_write3(0);
		 else if (command3 == 0)	// Return
 			 state = ST_CONTROL;
		 break;

	 case ST_LIGHT1:	// LIGHTS FOR ROOM 1 STATE
	 	 control_light1();			// Prompt for Lights ON or OFF
		 int command4 = control_keypad();	// Get value from keypad
	 	 if (command4 == 1)			// Lights ON
	 		 state = ST_BRIGHT1;
		 else if (command4 == 2)	// Lights OFF
			 DAC_write1(0);
		 else if (command4 == 0)	// Return
 			 state = ST_ROOM1;
		 else if (command4 == 9)	// Home
			 state = ST_CONTROL;
		 break;

	 case ST_BRIGHT1:	// BRIGHT ROOM 1 STATE
		 DAC_write1(light1);		  	// Turn on Lights to previously store value
		 control_brightness();			// Prompt for Brighting of Dimming
		 int key1 = control_keypad(); 	// Get value from keypad
		 if (key1 == 1) {				// Brighting up to 4000mV MAX
			 if (light1 < MAX_BRIGHTNESS) {
				 light1 += BRIGHT_CHANGE;
				 DAC_write1(light1);
			 }
		 }
		 else if (key1 == 2) {			// Dimming up to 2000mV MIN
			 if (light1 > MIN_BRIGHTNESS) {
				 light1 -= BRIGHT_CHANGE;
				 DAC_write1(light1);
			 }
		 }
		 else if (key1 == 0)			// Return
			 state = ST_LIGHT1;
		 else if (key1 == 9)			// Home
			 state = ST_CONTROL;
		 break;

	 case ST_BRIGHT2:				// BRIGHT ROOM 2 STATE
		 DAC_write2(light2);			// Turn on Lights to previously store value
		 control_brightness();			// Prompt for Brighting of Dimming
		 int key2 = control_keypad();	// Get value from keypad
		 if (key2 == 1) {				// Brighting up to 4000mV MAX
			 if (light2 < MAX_BRIGHTNESS) {
				 light2 += BRIGHT_CHANGE;
				 DAC_write2(light2);
			 }
		 }
		 else if (key2 == 2) {			// Dimming up to 2000mV MIN
			 if (light2 > MIN_BRIGHTNESS) {
				 light2 -= BRIGHT_CHANGE;
				 DAC_write2(light2);
			 }
		 }
		 else if (key2 == 0)			// Return
			 state = ST_ROOM2;
		 else if (key2 == 9)			// Home
			 state = ST_CONTROL;
		 break;

	 case ST_BRIGHT3:				// BRIGHT ROOM 3 STATE
		 DAC_write3(light3);			// Turn on Lights to previously store value
		 control_brightness();			// Prompt for Brighting of Dimming
		 int key3 = control_keypad();	// Get value from keypad
		 if (key3 == 1) {				// Brighting up to 4000mV MAX
			 if (light3 < MAX_BRIGHTNESS) {
				 light3 += BRIGHT_CHANGE;
				 DAC_write3(light3);
			 }
		 }
		 else if (key3 == 2) {			// Dimming up to 2000mV MIN
			 if (light3 > MIN_BRIGHTNESS) {
				 light3 -= BRIGHT_CHANGE;
				 DAC_write3(light3);
			 }
		 }
		 else if (key3 == 0)			// Return
			 state = ST_ROOM3;
		 else if (key3 == 9)			// Home
			 state = ST_CONTROL;
		 break;

	 case ST_FAN:						// FAN STATE
		 control_fan();					// Prompt for fan ON/OFF
		 int key4 = control_keypad();	// Get value from keypad
		 if (key4 == 1)					// Turn on/off
			 MOTOR_PORT -> ODR ^= GPIO_PIN_0;
		 else if (key4 == 0)			// Return
			 state = ST_ROOM1;
		 else if (key4 == 9)			// Home
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
