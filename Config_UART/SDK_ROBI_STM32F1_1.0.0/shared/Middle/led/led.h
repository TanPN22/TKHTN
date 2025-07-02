/*******************************************************************************
 *
 * Copyright (c) 2024
 * Lumi, JSC.
 * All Rights Reserved
 *
 *
 * Description: Include file for application
 *
 * Author: ThangTV
 *
 * Last Changed By:  $Author: ThangTV $
 * Revision:         $Revision: 2.0 $
 * Last Changed:     $Date: 07/03/2024 $
 *
 ******************************************************************************/
#ifndef _LED_H_
#define _LED_H_
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include <stdint.h>
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
/* Compute the value to be set in ARR register to generate signal frequency at 17.57 KHz */
#define LED_TIMER_PERIOD           (SystemCoreClock / 17570) - 1

#define LED_PORT             		GPIOA

#define LED_GREEN_PIN               GPIO_Pin_5
#define LED_RED_PIN             	GPIO_Pin_6
#define LED_BLUE_PIN            	GPIO_Pin_7

#define NUM_OF_COLOR                3u

/*! @brief Led color */
typedef enum __led_color__ {
	LED_COLOR_RED = 0x00u,
	LED_COLOR_GREEN = 0x01u,
	LED_COLOR_BLUE = 0x02u,
//	LED_COLOR_WHITE = 0x03u,
//	LED_COLOR_BLACK = 0x04u,
//	LED_COLOR_YELLOW = 0x05u,
//	LED_COLOR_PINK = 0x06u,
//	LED_COLOR_CYAN = 0x07u,
} led_color_t;

/*! @brief Led level */
typedef uint8_t led_level_t;

#define isTypeLED(x)                ((x == LED_COLOR_RED) || (x == LED_COLOR_GREEN) || (x == LED_COLOR_BLUE))

/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/
/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/
/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/

/**
 * @func   LedInit
 * @brief  Initializes module led
 * @param  None
 * @retval None
 */
void
LedControl_Init(void);

/**
 * @func   	LedControl_SetColor
 * @brief  	Set color for LED
 * @param  	led_color: color of LED (LED_COLOR_RED, LED_COLOR_BLUE, LED_COLOR_GREEN)
 * 			led_level: brightness level of LED (0-100) (except LED_COLOR_GREEN)
 * @retval 	None
 */
void
LedControl_SetColor(
	led_color_t led_color,
	uint8_t led_level
);

/**
 * @func   LedToggle
 * @brief  Toggle led
 * @param  led_color: color of LED (LED_COLOR_RED, LED_COLOR_BLUE, LED_COLOR_GREEN)
 * @retval None
 */
void LedToggle(led_color_t led_color);

/**
 * @func   Toggle_LedYellow
 * @brief  Toggle_Led 3 color Yellow
 * @param  None
 * @retval None
 */
void Toggle_LedYellow(void);

/**
 * @func   Toggle_LedRed
 * @brief  Toggle_Led 3 color Red
 * @param  None
 * @retval None
 */
void Toggle_LedRed(void);

/**
 * @func   LedControl_Blink
 * @brief  LedControl_Blink
 * @param  	led_color: color of LED (LED_COLOR_RED, LED_COLOR_BLUE, LED_COLOR_GREEN)
 * 			led_level: brightness level of LED (0-100) (except LED_COLOR_GREEN)
 * 			numblink: Number of blinks ( > 0)
 * 			interval: Cycle of blink
 * @retval None
 */
void LedControl_Blink(led_color_t led_color, uint8_t led_level, uint8_t numblink, uint16_t interval);

#endif

/* END FILE */
