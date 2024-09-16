/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "../../Games/Game_2048.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

#define HIGH GPIO_PIN_SET
#define LOW GPIO_PIN_RESET
#define I2C_TIMEOUT 2000


// keep this undefined if you want afterglow protection
//#define afterglow

#define TLC_ADDRESS 0xC0
#define TLC_PWM_MODE 0xFF // see section 9.6.6 of the led driver datasheet
#define TLC_OSC_OFF 0x10
#define TLC_OSC_ON 0x01

// TLC Registers
#define MODE1 0x00 // w/o auto increment
#define LEDOUT0 0x94 // w/ auto increment
#define PWM0 0xA2 // w/ auto increment


// PORT A GPIOs
#define BUTTON_D1 GPIO_PIN_11
#define BUTTON_U1 GPIO_PIN_10
#define BUTTON_R1 GPIO_PIN_12
#define BUTTON_L1 GPIO_PIN_9
#define BUTTON_U2 GPIO_PIN_0
#define BUTTON_R2 GPIO_PIN_1
#define BUTTON_D2 GPIO_PIN_2
#define BUTTON_L2 GPIO_PIN_3
#define TLC_RESET GPIO_PIN_15
#define BUTTON_PORT GPIOA


// PORT B GPIOs
#define SCORE_DAT GPIO_PIN_0
#define SCORE_LAT GPIO_PIN_1
#define SCORE_CLK GPIO_PIN_2
#define MUX_1 GPIO_PIN_3
#define MUX_2 GPIO_PIN_4
#define MUX_3 GPIO_PIN_5
#define MUX_4 GPIO_PIN_6
#define MUX_5 GPIO_PIN_7
#define MUX_PORT GPIOB


// PORT C GPIOs
#define LED_R_DEBUG GPIO_PIN_13
#define LED_B_DEBUG GPIO_PIN_14
#define LED_G_DEBUG GPIO_PIN_15




/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */


const uint16_t BUTTON_LIST[8] = {
		  BUTTON_U1,
		  BUTTON_R1,
		  BUTTON_D1,
		  BUTTON_L1,
		  BUTTON_U2,
		  BUTTON_R2,
		  BUTTON_D2,
		  BUTTON_L2
};


const uint8_t COLOURS[45] = {
		  0, 0, 0, // null, 0
		  2, 0, 0, // red, ID 1
		  2, 2, 0, // yellow, ID 2
		  0, 2, 0, // green, ID 3
		  0, 2, 2, // cyan, ID 4
		  0, 0, 2, // blue, ID 5
		  2, 0, 2, // magenta, ID 6
		  2, 2, 2, // white, ID 7
		  2, 1, 0, // orange, ID 8
		  1, 0, 2, // purple, ID 9
		  1, 2, 0, // lime, ID 10
		  0, 1, 2, // azure, ID 11
		  0, 2, 1, // turqoise, ID 12
		  2, 0, 1, // rose, ID 13
		  3, 1, 2, // pink, ID 14
};


/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */


int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */

  HAL_GPIO_WritePin(GPIOC, LED_R_DEBUG, LOW);
  HAL_GPIO_WritePin(GPIOC, LED_B_DEBUG, LOW);
  HAL_GPIO_WritePin(GPIOC, LED_G_DEBUG, LOW);



  // Turns off all multiplexer pins
  HAL_GPIO_WritePin(MUX_PORT, MUX_1, HIGH);
  HAL_GPIO_WritePin(MUX_PORT, MUX_2, HIGH);
  HAL_GPIO_WritePin(MUX_PORT, MUX_3, HIGH);
  HAL_GPIO_WritePin(MUX_PORT, MUX_4, HIGH);
  HAL_GPIO_WritePin(MUX_PORT, MUX_5, HIGH);




  // colour ID system --> uint8_t, BBBBIIII
  // [3:0] --> colour identifier
  // [7:4] --> brightness


  /*
#define RED_1 0b00000001
#define YEL_1 0b00000010
#define GRN_1 0b00000011
#define CYN_1 0b00000100
#define BLU_1 0b00000101
#define MAG_1 0b00000110
#define WHT_1 0b00000111

#define RED_2 0b00010001
#define YEL_2 0b00010010
#define GRN_2 0b00010011
#define CYN_2 0b00010100
#define BLU_2 0b00010101
#define MAG_2 0b00010110
#define WHT_2 0b00010111

#define RED_3 0b00100001
#define YEL_3 0b00100010
#define GRN_3 0b00100011
#define CYN_3 0b00100100
#define BLU_3 0b00100101
#define MAG_3 0b00100110
#define WHT_3 0b00100111
   */







  // init variables that hold button data



  //uint8_t rainbow_on[17] = {PWM0, 0xFF, 0, 0, 0xFF, 0xFF, 0, 0, 0xFF, 0, 0, 0xFF, 0xFF, 0, 0, 0xFF, 0};
  //uint8_t rainbow_mid[17] = {PWM0, 16, 0, 0, 16, 16, 0, 0, 16, 0, 0, 16, 16, 0, 0, 16, 0};
  //uint8_t rainbow_dim[17] = {PWM0, 8, 0, 0, 8, 8, 0, 0, 8, 0, 0, 8, 8, 0, 0, 8, 0};
  //uint8_t all_off[17] = {PWM0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  //uint8_t all_dim[17] = {PWM0, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16};
  //uint8_t rain1[17] = {PWM0, 32, 0, 0, 100, 30, 0, 20, 20, 0, 0, 32, 0, 0, 24, 12, 0};


  // init buffers
  uint8_t grid[25] =
 		  {0, 0, 0, 0, 0,
 		   0, 0, 0, 0, 0,
 		   0, 0, 0, 0, 0,
 		   0, 0, 0, 0, 0,
 		   0, 0, 0, 0, 0
 		  };
  uint8_t row1_buf[17], row2_buf[17], row3_buf[17], row4_buf[17], row5_buf[17];
  uint8_t* matrix[5] = {row1_buf, row2_buf, row3_buf, row4_buf, row5_buf};
  for (int r = 0; r < 5; r++)
  	  matrix[r][0] = PWM0;
  set_grid(matrix, grid, 8);


  // LED driver initalization
  TLC_init();










  HAL_GPIO_WritePin(GPIOC, LED_B_DEBUG, HIGH);
  HAL_GPIO_WritePin(GPIOC, LED_G_DEBUG, HIGH);

  //set_pixel(matrix, 12, 0x01, 8);
  uint8_t test = 0;
  uint32_t test_time_diff = HAL_GetTick();
  uint32_t status_diff_millis = HAL_GetTick();


  Game_2048* test_2048 = new Game_2048(5, 5); // TODO test with other grids, like 4x4, 4x5, etc


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

	  /*
	  if (prev_button_states != button_states) {


		  if ((button_states&0x01) == 0 && (prev_button_states&0x01) != 0) {
			  solid_colour_grid(grid, 0x06);
			  set_grid(matrix, grid, 8);
		  }
		  else if ((button_states&0x02) == 0 && (prev_button_states&0x02) != 0)
		  {
			  solid_colour_grid(grid, 0x02);
			  set_grid(matrix, grid, 8);
		  }
		  else if ((button_states&0x04) == 0 && (prev_button_states&0x04) != 0)
		  {
			  solid_colour_grid(grid, 0x04);
			  set_grid(matrix, grid, 8);
		  }
		  else if ((button_states&0x08) == 0 && (prev_button_states&0x08) != 0)
		  {
			  solid_colour_grid(grid, 0x07);
			  set_grid(matrix, grid, 8);
		  }





	  }
	  */


	  test_2048->main_2048(matrix, grid);

	  /*
	if (HAL_GetTick() - test_time_diff > 50)
	{
	  	set_pixel(matrix, test, 0x01, 8);
	  	test = (test+1)%25;
	  	test_time_diff = HAL_GetTick();
	}
		*/





  }

  HAL_GPIO_WritePin(GPIOA, TLC_RESET, LOW);
  HAL_PWR_EnableSleepOnExit();


  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_I2C1;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_HSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

	  /* USER CODE BEGIN I2C1_Init 0 */

	  /* USER CODE END I2C1_Init 0 */

	  /* USER CODE BEGIN I2C1_Init 1 */

	  /* USER CODE END I2C1_Init 1 */
	  hi2c1.Instance = I2C1;
	  hi2c1.Init.Timing = 0x0000020B;
	  hi2c1.Init.OwnAddress1 = 0;
	  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	  hi2c1.Init.OwnAddress2 = 0;
	  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
	  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
	  {
	    Error_Handler();
	  }

	  /** Configure Analogue filter
	  */
	  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
	  {
	    Error_Handler();
	  }

	  /** Configure Digital filter
	  */
	  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
	  {
	    Error_Handler();
	  }
	  /* USER CODE BEGIN I2C1_Init 2 */

	  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_15, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6
                          |GPIO_PIN_7, GPIO_PIN_RESET);

  /*Configure GPIO pin : PC13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PA0 PA1 PA2 PA3
                           PA8 PA9 PA10 PA11
                           PA12 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
                          |GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11
                          |GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PA4 PA5 PA15 */
  GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PB15 */
  GPIO_InitStruct.Pin = GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PB3 PB4 PB5 PB6
                           PB7 */
  GPIO_InitStruct.Pin = GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6
                          |GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

void TLC_init() {

	// Reset pin must be held high for LED driver to not be standby mode
	HAL_GPIO_WritePin(GPIOA, TLC_RESET, LOW);
	HAL_GPIO_WritePin(GPIOA, TLC_RESET, HIGH);

	uint8_t buffer[2] = {MODE1, TLC_OSC_ON}; // Initialized to MODE1 register init; see section 9.6.1
	HAL_I2C_Master_Transmit(&hi2c1, TLC_ADDRESS, buffer, 2, HAL_MAX_DELAY);

#ifdef afterglow
	row_on();
#endif
}


void row_off() {
	uint8_t buffer[5] = {LEDOUT0, 0x00, 0x00, 0x00, 0x00};
	HAL_I2C_Master_Transmit(&hi2c1, TLC_ADDRESS, buffer, 5, HAL_MAX_DELAY);
}

void row_on() {
	uint8_t buffer[5] = {LEDOUT0, TLC_PWM_MODE, TLC_PWM_MODE, TLC_PWM_MODE, TLC_PWM_MODE};
	HAL_I2C_Master_Transmit(&hi2c1, TLC_ADDRESS, buffer, 5, HAL_MAX_DELAY);
}


void LED_heartbeat() {
	static uint8_t led_on = 0;

	if (led_on) {
		HAL_GPIO_WritePin(GPIOC, LED_R_DEBUG, HIGH);
		led_on = 0;
	}
	else {
		HAL_GPIO_WritePin(GPIOC, LED_R_DEBUG, LOW);
		led_on = 1;
	}

}


void set_LEDs(uint8_t* row_buffer) {


#ifndef afterglow
	row_off();
#endif

	// sets the colours of the LEDs
	HAL_I2C_Master_Transmit(&hi2c1, TLC_ADDRESS, row_buffer, 17, HAL_MAX_DELAY);
}

void set_grid(uint8_t** rows, uint8_t* grid, uint8_t bright_modifier) {

	uint8_t grid_index = 0, r, c, i, brightness, colour_id;

	uint8_t col, test;

	for (r = 0; r < 5; r++) {

		for (c = 0; c < 5; c++) {

			brightness =((grid[grid_index]>>4) + 1) * bright_modifier - 1;
			colour_id = (grid[grid_index] & 0b00001111)*3;

			for (i = 0; i < 3; i++) {
				rows[r][(c*3) + i + 1] = COLOURS[colour_id+i] * brightness;;
			}

			grid_index++;
		}
	}
}

void set_pixel(uint8_t** rows, uint8_t pixel, uint8_t colour, uint8_t bright_modifier) {


	if (pixel >= 25)
		return;

	uint8_t r = pixel/5, c = pixel%5, brightness, colour_id;

	brightness =((colour>>4) + 1) * bright_modifier - 1;
	colour_id = (colour & 0b00001111)*3;

	for (int i = 0; i < 3; i++) {
		rows[r][(c*3) + i + 1] = COLOURS[colour_id+i] * brightness;;
	}



}

void solid_colour_grid(uint8_t* grid, uint8_t colour) {
	for (int i = 0; i < 25; i++) {
		grid[i] = colour;
	}
}

void display(uint8_t** rows) {


	for (int k = 0; k < 5; ++k) {
		set_LEDs(rows[k]);
		HAL_GPIO_WritePin(MUX_PORT, MUX_1 << ((k+4)%5), HIGH);
		HAL_GPIO_WritePin(MUX_PORT, MUX_1 << k, LOW);
	#ifndef afterglow
		row_on();
	#endif

	}
}

uint8_t update_button_states(uint8_t current_states, uint32_t* time_diff) {

	uint8_t new_states = 0x00;
	uint32_t* temp;

	for (uint8_t btn = 0; btn < 8; btn++) {
		temp = &time_diff[btn];
		new_states |= ((query_button_change(temp, BUTTON_LIST[btn], (current_states>>btn)&0x01))<<btn);
	}


	return current_states ^ new_states;
}

uint8_t query_button_change(uint32_t* last_pressed, uint16_t button, uint8_t current_state) {


	if (HAL_GPIO_ReadPin(BUTTON_PORT, button) != current_state
			&& HAL_GetTick() - *last_pressed > 10)
	{
		*last_pressed = HAL_GetTick();
		return 1;
	}


	return 0;
}

void disable_mux5() {
	HAL_GPIO_WritePin(MUX_PORT, MUX_5, HIGH);
}



/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
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
