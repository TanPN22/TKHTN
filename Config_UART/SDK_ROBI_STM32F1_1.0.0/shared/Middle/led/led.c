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
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"
#include "led.h"
#include "utilities.h"
#include "timer.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/
static led_level_t g_brightnessLed = 0;
//static uint8_t ledBuff = 1;
static uint8_t ledYeBuff = 1, ledReBuff = 1;
uint16_t PrescalerValue = 8;
uint16_t TIMPeriod = 200;

uint32_t blinkBuffer;
uint8_t logicBlink = 0;
/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/
/******************************************************************************/
/*                            PRIVATE FUNCTIONS                               */
/******************************************************************************/
static void LedGreenGPIO_Config(void);
static void TIMLED_Configuration(void);
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
LedControl_Init(void) {
	// Initialize pins GPIO led
	LedGreenGPIO_Config();
	TIMLED_Configuration();
}

/**
  * @func   LedGreenGPIO_Config
  * @brief  Configure GPIO for Led Green
  * @param  None
  * @retval None
  */
static
void LedGreenGPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* GPIOA Clocks enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	/* GPIOA Configuration as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = LED_GREEN_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(LED_PORT, &GPIO_InitStructure);
}

/**
 * @func TIMLED_Configuration
 * @brief Initialize time to control LED Blue and Red
 * @param None
 * @retval None
 */
static
void TIMLED_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = LED_RED_PIN | LED_BLUE_PIN;
	GPIO_Init(LED_PORT, &GPIO_InitStructure);

	/* Compute the prescaler value */
	PrescalerValue = (uint16_t) (SystemCoreClock / 24000000) - 1;

	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = TIMPeriod - 1;
	TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue - 1;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

	/* PWM1 Mode configuration: Channel1 */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Set;

	TIM_OC1Init(TIM3, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);

	TIM_OC2Init(TIM3, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);

    TIM_CtrlPWMOutputs(TIM3, ENABLE);

	TIM_ARRPreloadConfig(TIM3, ENABLE);

	/* TIM3 enable counter */
	TIM_Cmd(TIM3, ENABLE);
	TIM_SetCompare1(TIM3, 65535);
	TIM_SetCompare2(TIM3, 65535);
}

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
) {
	uint32_t value;
	uint32_t period = TIM3->ARR;

	if (led_level > 100) return;

	value = ((100 - led_level) * period) / 100;

	switch (led_color) {
		case LED_COLOR_RED:
			TIM_SetCompare1(TIM3, value);
			break;
		case LED_COLOR_BLUE:
			TIM_SetCompare2(TIM3, value);
			break;
		case LED_COLOR_GREEN:
			if (led_level != 0) {
				GPIO_SetBits(LED_PORT, LED_GREEN_PIN);
			}
			else if (led_level == 0){
				GPIO_ResetBits(LED_PORT, LED_GREEN_PIN);
			}
			break;
		default:
			break;
	}
}

/**
 * @func   LedToggle
 * @brief  Toggle led
 * @param  led_id: identify of led
 *         led_color: color of led
 * @retval None
 */
void LedToggle(led_color_t led_color)
{
    uint8_t led_level;
    static uint8_t bToggleAll = 0;

	if (bToggleAll == 0)
	{
		led_level = g_brightnessLed;
	}
	else /* (bToggleAll == 1) */
	{
		led_level = 0;
	}
	bToggleAll = !bToggleAll;
	LedControl_SetColor(led_color, led_level);
}

/**
 * @func   Toggle_LedYellow
 * @brief  Toggle_Led 3 color Yellow
 * @param  None
 * @retval None
 */
void Toggle_LedYellow(void)
{
	switch (ledYeBuff) {
		case 1:
			LedControl_SetColor(LED_COLOR_BLUE, 0);
			LedControl_SetColor(LED_COLOR_RED, 100);
			LedControl_SetColor(LED_COLOR_GREEN, 100);
			ledYeBuff = 2;
			break;
		case 2:
			LedControl_SetColor(LED_COLOR_BLUE, 0);
			LedControl_SetColor(LED_COLOR_RED, 0);
			LedControl_SetColor(LED_COLOR_GREEN, 0);
			ledYeBuff = 1;
			break;

		default:
			break;
	}
}

/**
 * @func   Toggle_LedRed
 * @brief  Toggle_Led 3 color Red
 * @param  None
 * @retval None
 */
void Toggle_LedRed(void)
{
	switch (ledReBuff) {
		case 1:
			LedControl_SetColor(LED_COLOR_BLUE, 0);
			LedControl_SetColor(LED_COLOR_RED, 100);
			LedControl_SetColor(LED_COLOR_GREEN, 0);
			ledReBuff = 2;
			break;
		case 2:
			LedControl_SetColor(LED_COLOR_BLUE, 0);
			LedControl_SetColor(LED_COLOR_RED, 0);
        	LedControl_SetColor(LED_COLOR_GREEN, 0);
			ledReBuff = 1;
			break;

		default:
			break;
	}
}

/**
 * @func   LedControl_Blink
 * @brief  LedControl_Blink
 * @param  	led_color: color of LED (LED_COLOR_RED, LED_COLOR_BLUE, LED_COLOR_GREEN)
 * 			led_level: brightness level of LED (0-100) (except LED_COLOR_GREEN)
 * 			numblink: Number of blinks ( > 0)
 * 			interval: Cycle of blink
 * @retval None
 */
void LedControl_Blink(led_color_t led_color, uint8_t led_level, uint8_t numblink, uint16_t interval)
{
	static uint8_t count = 0;
	if (count == numblink*2)return;
	if (GetMilSecTick() - blinkBuffer > 500)
	{
		switch (logicBlink) {
			case 0:
				LedControl_SetColor(led_color, 0);
				logicBlink = 1;
				break;
			case 1:
				LedControl_SetColor(led_color, led_level);
				logicBlink = 0;
				break;
			default:
				break;
		}
		blinkBuffer = GetMilSecTick();
		count ++;
	}
}

/* END FILE */
