/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
extern uint8_t countGlobal;
extern uint8_t flag_exti1;
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define ROW1_Pin GPIO_PIN_4
#define ROW1_GPIO_Port GPIOA
#define ROW2_Pin GPIO_PIN_5
#define ROW2_GPIO_Port GPIOA
#define ROW3_Pin GPIO_PIN_6
#define ROW3_GPIO_Port GPIOA
#define ROW4_Pin GPIO_PIN_7
#define ROW4_GPIO_Port GPIOA
#define COL5_Pin GPIO_PIN_0
#define COL5_GPIO_Port GPIOB
#define COL6_Pin GPIO_PIN_1
#define COL6_GPIO_Port GPIOB
#define COL7_Pin GPIO_PIN_2
#define COL7_GPIO_Port GPIOB
#define ROW5_Pin GPIO_PIN_8
#define ROW5_GPIO_Port GPIOA
#define COL1_Pin GPIO_PIN_9
#define COL1_GPIO_Port GPIOA
#define COL2_Pin GPIO_PIN_10
#define COL2_GPIO_Port GPIOA
#define COL3_Pin GPIO_PIN_11
#define COL3_GPIO_Port GPIOA
#define COL4_Pin GPIO_PIN_12
#define COL4_GPIO_Port GPIOA

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
