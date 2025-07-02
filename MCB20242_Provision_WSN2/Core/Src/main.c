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
#include "lora.h"
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_RTC_Init(void);
static void MX_SPI1_Init(void);
static void MX_USART3_UART_Init(void);
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
RTC_HandleTypeDef hrtc;

SPI_HandleTypeDef hspi1;

UART_HandleTypeDef huart3;


/* USER CODE BEGIN PV */
int _write(int fd, char * ptr, int len) {
  HAL_UART_Transmit( & huart3, (uint8_t * ) ptr, len, HAL_MAX_DELAY);
  return len;
}

#define SEARCH_NODE_TIMEOUT		2000
#define MAX_NODES 20
#define MAX_RETRIES 3
#define DATA_PACKET_TIMEOUT_MS 1000
#define TIME_CYCLE_GET 1

 lora_pins_t lora_pins;												// Structure variable for lora pins
 lora_t lora;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
int nodeCount = 0;
typedef struct {
     uint8_t id;
     float x;
     float y;
} node_info_t;
node_info_t nodes[MAX_NODES];

char data_receive[80];
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void lora_Init(void){
	//cấu hình lora
	lora_pins.dio0.port  = LORA_DIO0_PORT;
	lora_pins.dio0.pin   = LORA_DIO0_PIN;
	lora_pins.nss.port   = LORA_SS_PORT;				// NSS pin to which port is connected
	lora_pins.nss.pin    = LORA_SS_PIN;					// NSS pin to which pin is connected
	lora_pins.reset.port = LORA_RESET_PORT;			// RESET pin to which port is connected
	lora_pins.reset.pin  = LORA_RESET_PIN;			// RESET pin to which pin is connected
	lora_pins.spi  			 = &hspi1;
	lora.pin = &lora_pins;
	lora.frequency = FREQ_433MHZ;								// 433MHZ Frequency
	//lora.frequency = FREQ_865MHZ;								// 865MHZ Frequency
	//lora.frequency = FREQ_866MHZ;								// 866MHZ Frequency
	//lora.frequency = FREQ_867MHZ;								// 867MHZ Frequency
	lora.bandwidth = BW_125KHz;
	lora.powergain = POWER_20db;
	lora.spreadingfactor = SF_7;
	lora.codingrate = CR_4_5;


	while(lora_init(&lora)){										// Initialize the lora module
		HAL_Delay(1000);
	}

}

char dataBroadcast[] = "PROV";
void send_broadcast(){
	lora_begin_packet(&lora);
	lora_tx(&lora, (uint8_t*)dataBroadcast, strlen(dataBroadcast));
	lora_end_packet(&lora);
}

int node_id;

void send_ID(uint8_t node_id) {
    char buf[64];
    sprintf(buf, "%d %d %f %f",node_id, nodeCount++, 24.5, 22.6);
    lora_begin_packet(&lora);
    lora_tx(&lora, (uint8_t*)buf, strlen(buf));
    lora_end_packet(&lora);
    printf("Send ID to %d: %s\r\n", node_id, buf);
}

int search_Node(){
	send_broadcast();
	char buf[60];
	uint32_t timeStart = HAL_GetTick();
	while (HAL_GetTick() - timeStart <= SEARCH_NODE_TIMEOUT){
		uint8_t ret = lora_prasePacket(&lora);
		uint8_t i = 0;
		if (ret) {
		i = 0;
		while (lora_available(&lora) && i < sizeof(buf) - 1) {
			buf[i++] = lora_read(&lora);
		}
		buf[i] = '\0';

		if (strncmp(buf, "ACK", 3) == 0) {
			sscanf(buf + 4, "%d", &node_id);

			return 1;
			}
		}
	}
	return 0;
}


void send_data(char* data){
	// Gửi gói tin data
	lora_begin_packet(&lora);
	lora_tx(&lora, (uint8_t *)data, strlen(data));
	lora_end_packet(&lora);
}

int receive_data(uint8_t *node_id, float *t, float *h, uint8_t nodeid_i) {
	uint8_t buf[60];
//    uint8_t ret = lora_prasePacket(&lora);
	uint8_t ret =0;

	uint32_t timeStart = HAL_GetTick();

	while(!ret){
	  ret = lora_prasePacket(&lora);
	  if(ret){
	   //Nhận gói tin
	   uint8_t i = 0;
	   memset(buf, 0, sizeof(buf));

	    while (lora_available(&lora)) {
	    	buf[i++] = lora_read(&lora);
	   }
	    buf[i] = '\0'; // Kết thúc chuỗi

	    sscanf(buf, "%d %f %f", node_id, t, h);

	    return 1;
	  }

	if (HAL_GetTick() - timeStart >= 2000){
		send_request(nodeid_i);
		timeStart = HAL_GetTick();
	}
	}
    return 0;
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

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_RTC_Init();
  MX_SPI1_Init();
  MX_USART3_UART_Init();
  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_11,GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_10,GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_9,GPIO_PIN_SET);

  /* USER CODE BEGIN 2 */
  lora_Init();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE BEGIN 3 */
	  uint32_t timeSearch = HAL_GetTick();
	  while (HAL_GetTick() - timeSearch <= 5000){
		  search_Node();
	  }

	  for (int i = 0; i <= TIME_CYCLE_GET; i ++){
		  HAL_Delay(1000);
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
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
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */

  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.AsynchPrediv = RTC_AUTO_1_SECOND;
  hrtc.Init.OutPut = RTC_OUTPUTSOURCE_ALARM;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

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
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, Data_DHT_Pin|NSS_Pin|Led_6_Pin|Led_5_Pin|Led_4_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(RST_GPIO_Port, RST_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : Data_DHT_Pin NSS_Pin Led_5_Pin Led_4_Pin */
  GPIO_InitStruct.Pin = Data_DHT_Pin|NSS_Pin|Led_6_Pin|Led_5_Pin|Led_4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : DIO0_Pin */
  GPIO_InitStruct.Pin = DIO0_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(DIO0_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : RST_Pin */
  GPIO_InitStruct.Pin = RST_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(RST_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
//void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
//	sprintf(data_receive,"Da vao ngat\r\n");
//	HAL_UART_Transmit(&huart3, (uint8_t *)data_receive, strlen(data_receive),1000);
//	if(GPIO_Pin = DIO0_Pin){
//		uint8_t i=0;
//		while (lora_available(&lora)) {
//				buf[i++] = lora_read(&lora);
//				}
//				buf[i] = '\0'; // Kết thúc chuỗi
//				sprintf(data_receive,"%s\r\n",buf);
//				HAL_UART_Transmit(&huart3, (uint8_t *)data_receive, strlen(data_receive),1000);
//
//	}
//}
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
