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
#include "dht22.h"
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

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
RTC_HandleTypeDef hrtc;

SPI_HandleTypeDef hspi1;

UART_HandleTypeDef huart3;

/* USER CODE BEGIN PV */
char msg[64],buf[60];
char data_receive[80], data_th[40],data_register[30];
typedef struct {
    int id ;     // ID của Node
    float x;    // Tọa độ x
    float y;    // Tọa độ y
} Node;
Node node ={4,41.75165,42.36589};

int id_receive,stt;
int timeout_listen_request_data =0;
int restart = 0;
uint32_t now_tick,start_tick,end_tick;
float t_min, t_max, h_min, h_max,t_last,t,h;
 float T = 0, H = 0;
/* USER CODE BEGIN PV */
#ifdef	__GNUC__
		#define PUTCHAR_PROTOTYPE int	__io_putchar(int ch)
#else
		#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif
 PUTCHAR_PROTOTYPE
{
	HAL_UART_Transmit(&huart3, (uint8_t *)&ch,1,0xFFFF);
	return ch;
}
 lora_pins_t lora_pins;												// Structure variable for lora pins
 lora_t lora;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_RTC_Init(void);
static void MX_SPI1_Init(void);
static void MX_USART3_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void listen_Broadcast(void){
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
	sprintf(msg,"Configuring LoRa module\r\n");
	HAL_UART_Transmit(&huart3,(uint8_t *)msg,strlen(msg),1000);

	while(lora_init(&lora)){										// Initialize the lora module
		sprintf(msg,"LoRa Failed\r\n");
		HAL_UART_Transmit(&huart3,(uint8_t *)msg,strlen(msg),1000);
		HAL_Delay(1000);
	}
	sprintf(msg,"Done configuring LoRaModule\r\n");
	HAL_UART_Transmit(&huart3,(uint8_t *)msg,strlen(msg),1000);
	uint8_t ret =0;
	while(!ret){
	  ret = lora_prasePacket(&lora);
	  if(ret){
	   //Nhận gói tin
	   uint8_t i = 0;
	    while (lora_available(&lora)) {
	   buf[i++] = lora_read(&lora);
	   }
	   buf[i] = '\0'; // Kết thúc chuỗi
//	   sprintf(data_receive,"Da nhan duoc broadcast: %s\r\n",buf);
//	   HAL_UART_Transmit(&huart3, (uint8_t*)data_receive, strlen(data_receive), HAL_MAX_DELAY);
	   sprintf(data_receive,buf);
	   if(!strcmp(data_receive, "Open")){
		   break;
	   }
	   else{
		   ret =0;
	   }
	  }
	}
}

void send_register(char* data){
	// Gửi gói tin đăng ký
	lora_begin_packet(&lora);
	lora_tx(&lora, (uint8_t *)data, strlen(data));
	lora_end_packet(&lora);
//	sprintf(buf,"Da gui thong tin dang ky len Gateway\r\n");
//	HAL_UART_Transmit(&huart3, (uint8_t *)buf, strlen(buf), 1000);
}
bool check_and_parse_STT(const char *packet, int *id, int *stt, float *t_min, float *t_max, float *h_min, float *h_max) {
    // Kiểm tra định dạng gói tin
    if (sscanf(packet, "%d %d %f %f %f %f", id, stt, t_min, t_max, h_min, h_max) == 6) {
        // Kiểm tra tính hợp lệ của các giá trị
        return true; // Gói tin hợp lệ
    }
    return false; // Gói tin không hợp lệ
}
int listen_STT(void){
	uint8_t ret =0;
	now_tick = HAL_GetTick();
	while(!ret){
		ret = lora_prasePacket(&lora);
		if(ret){
			//Nhận gói tin
			uint8_t i = 0;
			while (lora_available(&lora)) {
				buf[i++] = lora_read(&lora);
			}
			buf[i] = '\0'; // Kết thúc chuỗi
			sprintf(data_receive,buf);
//			sprintf(buf,"Nhan duoc STT: %s\r\n",data_receive);
//		HAL_UART_Transmit(&huart3, (uint8_t *)buf, strlen(buf), 1000);
			if(check_and_parse_STT(data_receive, &id_receive, &stt, &t_min, &t_max, &h_min, &h_max)){
				if(node.id == id_receive){
//					sprintf(buf,"Nhan duoc STT hop le: %s\r\n",data_receive);
//					HAL_UART_Transmit(&huart3, (uint8_t *)buf, strlen(buf), 1000);
					return 1;
				}
				else{
//					sprintf(buf,"STT nhan ve co id khong khop\r\n");
//					HAL_UART_Transmit(&huart3, (uint8_t *)buf, strlen(buf), 1000);
					ret=0;
				}
		    }
			else{
//				sprintf(buf,"STT nhan duoc khong hop le\r\n");
//				HAL_UART_Transmit(&huart3, (uint8_t *)buf, strlen(buf), 1000);
				ret =0;
			}
		}
		if ((HAL_GetTick()-now_tick)>=300) //lắng nghe STT trong 100ms
					return 0;
	}
}

//Hàm check gói tin yêu cầu gửi data từ GW
bool check_request_data(const char *str, int targetId) {
    // Tạo buffer tạm thời để so sánh
    char idBuffer[16]; // Kích thước đủ để chứa id dạng chuỗi
    snprintf(idBuffer, sizeof(idBuffer), "%d R", targetId); // Tạo chuỗi "id R"
    // So sánh chuỗi nhận được với chuỗi mong đợi
    int x=strncmp(str, idBuffer, strlen(idBuffer));
    if (!x) {
    	return 1;
    }
    else {
    	return 0;
    }
}

int listen_request_data(void) {
    uint8_t ret = 0;
    uint32_t now_tick = HAL_GetTick();
    uint8_t i = 0;
    memset(buf, 0, sizeof(buf)); // Xóa buffer trước khi sử dụng
    while (!ret) {
    	if ((HAL_GetTick() - now_tick) >= 9000) {
//    	   sprintf(buf, "Time out on listen request data\r\n");
//    	   HAL_UART_Transmit(&huart3, (uint8_t *)buf, strlen(buf), 1000);
    	   restart = 1;
    	   break;
    	}
        ret = lora_prasePacket(&lora);
        if (ret) {
//        	sprintf(buf, "Dang nhan goi tin...\r\n");
//        	HAL_UART_Transmit(&huart3, (uint8_t *)buf, strlen(buf), 1000);
            // Nhận gói tin
            i = 0;
            while (lora_available(&lora) && i < sizeof(buf) - 1) {
                buf[i++] = lora_read(&lora);
            }
            buf[i] = '\0'; // Kết thúc chuỗi

            sprintf(data_receive, buf);
//            sprintf(buf, "Nhan duoc goi tin: %s\r\n",data_receive);
//            HAL_UART_Transmit(&huart3, (uint8_t *)buf, strlen(buf), 1000);
            if (check_request_data(data_receive, node.id)) {
//            	sprintf(buf, "Request hop le\r\n");
//            	HAL_UART_Transmit(&huart3, (uint8_t *)buf, strlen(buf), 1000);
                return 1;
            } else {
//            	sprintf(buf, "Request khong hop le\r\n");
//            	HAL_UART_Transmit(&huart3, (uint8_t *)buf, strlen(buf), 1000);
                ret = 0;
            }
        }


    }
    return 0; // Thêm giá trị trả về để tránh thiếu kết thúc
}


void send_data(char* data){
	// Gửi gói tin data
	lora_begin_packet(&lora);
	lora_tx(&lora, (uint8_t *)data, strlen(data));
	lora_end_packet(&lora);
//	sprintf(buf,"Da gui du lieu t&h len Gateway T=%.1f H=%.1f\r\n",T,H);
//	HAL_UART_Transmit(&huart3, (uint8_t *)buf, strlen(buf), 1000);
}

int listen_ok(void){
	uint8_t ret =0;
	now_tick=HAL_GetTick();
	while(!ret){
		if((HAL_GetTick()-now_tick)>=400){
//			sprintf(buf,"time out on listen_ok\r\n");
//			HAL_UART_Transmit(&huart3, (uint8_t *)buf, strlen(buf),1000);
			return 0;
		}
		ret = lora_prasePacket(&lora);
		if(ret){
		//Nhận gói tin
			uint8_t i = 0;
			while (lora_available(&lora)) {
				buf[i++] = lora_read(&lora);
			}
			buf[i] = '\0'; // Kết thúc chuỗi
			sprintf(data_receive,buf);
			sprintf(buf,"%d Ok",node.id);
			if(!strcasecmp(data_receive,buf)){
				start_tick=HAL_GetTick()-start_tick;
//				sprintf(buf,"Da nhan duoc Ok tu GW, tong thoi gian: %d\r\n",start_tick);
//				HAL_UART_Transmit(&huart3, (uint8_t *)buf, strlen(buf),1000);
				return 1;
			}
			else ret=0;
		}
	}
}

// Hàm đưa vi điều khiển vào chế độ ngủ trong thời gian tùy chọn (tính bằng giây)

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
  dht22_init();
  DHT22_GetTemp_Humidity(&T, &H);
  sprintf(data_th,"%d %.1f %.1f",node.id,T/2.0,H);

  sprintf(data_register, "%d %.5f %.5f",node.id,node.x,node.y);
  /* USER CODE BEGIN 2 */
  //DHT22_GetTemp_Humidity(&T, &H);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
	  restart =0;
	  int count_send_register =0;
	  int count_send_data=0;
	  listen_Broadcast();
	  send_register(data_register);
	  while(!listen_STT()){
		  if(count_send_register < 5){
			  send_register(data_register);
			  count_send_register++;
		  }
		  else{
			  restart =1;
			  break;
		  }
	  }
	  if(restart) continue;

	  listen_request_data();
	  if(restart) continue;
	  send_data(data_th);
	  while(!listen_ok()){
		  if(count_send_data <5){
			  send_data(data_th);
			  count_send_data++;
		  }
		  else{
			  restart=1;
			  break;
		  }
	  }
	  T=T/2.0;
	  	if( T> t_max)
	  	  	  	  	{
	  	  	  	  		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_11,GPIO_PIN_SET);
	  	  	  	  	}
	  	  	  	  	  else
	  	  	  	  	  {
	  	  	  	  		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_11,GPIO_PIN_RESET);
	  	  	  	  	  }
	  	  	  	  	if(T < t_min)
	  	  	  	  	{
	  	  	  	  		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_9,GPIO_PIN_SET);
	  	  	  	  	}
	  	  	  	  	  else
	  	  	  	  	  {
	  	  	  	  		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_9,GPIO_PIN_RESET);
	  	  	  	  	  }
	  	  	  	  	if (T >= t_min && T <= t_max)
	  	  	  	  	{
	  	  	  	  		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_10,GPIO_PIN_SET);
	  	  	  	  	}
	  	  	  	  	  else
	  	  	  	  	  {
	  	  	  	  		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_10,GPIO_PIN_RESET);
	  	  	  	  	  }
	  //if(restart) continue;

	  HAL_Delay(7000);
	  DHT22_GetTemp_Humidity(&T, &H);
	  sprintf(data_th,"%d %.1f %.1f",node.id,T/2.0,H);
    /* USER CODE BEGIN 3 */
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
