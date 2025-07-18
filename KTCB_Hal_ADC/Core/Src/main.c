/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include <stdio.h>
#include <string.h>
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define LCD_ADDR (0x27 << 1)  // STM32 HAL dùng địa chỉ 8-bit
// Các bit điều khiển
#define LCD_BACKLIGHT 0x08
#define LCD_ENABLE    0x04
#define LCD_RW        0x00  // luôn ghi
#define LCD_RS_DATA   0x01
#define LCD_RS_CMD    0x00
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;

I2C_HandleTypeDef hi2c1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ADC1_Init(void);
static void MX_I2C1_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
	uint16_t adc_hal_value = 0;

	uint8_t nowStatus = 0;
	uint8_t prevStatus = 0;
	uint32_t count = 0;
	uint32_t timeBuffer = 0;

	float speedValue = 0;

//	float speedData[100];
//	uint8_t dataCount = 0;


	void lcd_send_internal(uint8_t data, uint8_t flags) {
	    uint8_t upper = data & 0xF0;
	    uint8_t lower = (data << 4) & 0xF0;

	    uint8_t data_arr[4];
	    data_arr[0] = upper | flags | LCD_BACKLIGHT | LCD_ENABLE;
	    data_arr[1] = upper | flags | LCD_BACKLIGHT;
	    data_arr[2] = lower | flags | LCD_BACKLIGHT | LCD_ENABLE;
	    data_arr[3] = lower | flags | LCD_BACKLIGHT;

	    HAL_I2C_Master_Transmit(&hi2c1, LCD_ADDR, data_arr, 4, HAL_MAX_DELAY);
	}

	void lcd_send_cmd(uint8_t cmd) {
	    lcd_send_internal(cmd, LCD_RS_CMD);
	    HAL_Delay(1);
	}

	void lcd_send_data(uint8_t data) {
	    lcd_send_internal(data, LCD_RS_DATA);
	    HAL_Delay(1);
	}

	void lcd_init(void) {
	    HAL_Delay(50);
	    lcd_send_cmd(0x30);
	    HAL_Delay(5);
	    lcd_send_cmd(0x30);
	    HAL_Delay(1);
	    lcd_send_cmd(0x30);
	    HAL_Delay(10);
	    lcd_send_cmd(0x20);  // 4-bit mode

	    lcd_send_cmd(0x28);  // 2 lines, 5x8 font
	    lcd_send_cmd(0x08);  // display off
	    lcd_send_cmd(0x01);  // clear
	    HAL_Delay(2);
	    lcd_send_cmd(0x06);  // entry mode
	    lcd_send_cmd(0x0C);  // display on, cursor off, blink off
	}

	void lcd_clear(void) {
	    lcd_send_cmd(0x01);
	    HAL_Delay(2);
	}

	void lcd_set_cursor(uint8_t row, uint8_t col) {
	    uint8_t addr = (row == 0) ? 0x80 + col : 0xC0 + col;
	    lcd_send_cmd(addr);
	}

	void lcd_print(char *str) {
	    while (*str) {
	        lcd_send_data((uint8_t)(*str));
	        str++;
	    }
	}

	// -----------------------------
	// Hàm in float speedValue
	// -----------------------------
	void lcd_print_speed(float speedValue) {
	    char buffer[16];
	    snprintf(buffer, sizeof(buffer), "Speed: %.2f m/s", speedValue);

	    lcd_clear();
	    lcd_set_cursor(0, 0);
	    lcd_print(buffer);
	}
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
uint16_t Get_ADC_channel(uint32_t channel){
  	  uint16_t adc;
  //---------------Congig channel-------------

  	  ADC_ChannelConfTypeDef sConfig = {0};
  	  sConfig.Channel = channel;
  	  sConfig.Rank = ADC_REGULAR_RANK_16;
  	  sConfig.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;
  	  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  	  {
  	    Error_Handler();
  	  }

  //---------------Read value---------------
  	  HAL_ADC_Start(&hadc1);
  	  HAL_ADC_PollForConversion(&hadc1, 1000);
  	  adc = HAL_ADC_GetValue(&hadc1);
  	  HAL_ADC_Stop(&hadc1);
  	  return adc;
  }
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_ADC1_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
  lcd_init();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  adc_hal_value = Get_ADC_channel(ADC_CHANNEL_6);
	  nowStatus = adc_hal_value < 2000;

	  if (nowStatus != prevStatus){

		  if (nowStatus == 1)
		  {
			  count ++;
		  }

		  prevStatus = nowStatus;
	  }

	  if (HAL_GetTick() - timeBuffer >= 3000){
		  speedValue = (float)count * 0.0658 * 3.1415 / 2.0 / ((HAL_GetTick() - timeBuffer) / 1000.0);
		  lcd_print_speed(speedValue);

		  HAL_Delay(400);
//		  speedData[dataCount ++] = speedValue;
		  count = 0;
		  timeBuffer = HAL_GetTick();
	  }

  }
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
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL16;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Common config
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_6;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

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
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
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
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

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
