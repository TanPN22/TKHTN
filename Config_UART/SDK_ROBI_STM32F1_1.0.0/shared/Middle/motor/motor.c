/*******************************************************************************
 *
 * Copyright (c) 2023
 * Lumi, JSC.
 * All Rights Reserved
 *
 * Description: Include file for application
 *
 * Author: HoangNH
 *
 * Last Changed By:  $Author: TanPN $
 * Revision:         $Revision: $
 * Last Changed:     $Date: 01/08/2023 $
 *
*******************************************************************************/
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"
#include "motor.h"
#include "encoder.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
#define MOTOR_PWM_FREQUENCY     1000 // 1 kHz

#define MOTOR_TIM_PRESCALE      36
#define MOTOR_TIMER_RCC_FREQ    36000000
#define MOTOR_TIM_PERIOD        (MOTOR_TIMER_RCC_FREQ / (MOTOR_TIM_PRESCALE * MOTOR_PWM_FREQUENCY))

/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/
/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/
/******************************************************************************/
/*                            PRIVATE FUNCTIONS                               */
/******************************************************************************/
static void MOTOR_GPIO_Config(void);
/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/

/**
 * @func   MotorControl_Init
 * @brief  Initializes module motor
 * @param  None
 * @retval None
 */
void
MotorControl_Init(void)
{
	// Initialize pins GPIO motor
	MOTOR_GPIO_Config();
}

/**
 * @func   GPIO_Config
 * @brief  Configure GPIO for motor
 * @param  None
 * @retval None
 */
static
void MOTOR_GPIO_Config(void) {
	GPIO_InitTypeDef 			GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef 	TIM_TimeBaseInitStructure;
	TIM_OCInitTypeDef			TIM_OCInitStructure;

	/* Enable clock GPIOB */
	RCC_APB2PeriphClockCmd(MOTOR_RCC_GPIO, ENABLE);

	/* Initializes GPIO Use Output Compare */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	/* GPIOB Configuration as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = MOTOR_R1_GPIO_PIN | MOTOR_R2_GPIO_PIN |
								  MOTOR_L1_GPIO_PIN | MOTOR_L2_GPIO_PIN;
	GPIO_Init(MOTOR_GPIO_PORT, &GPIO_InitStructure);

	GPIO_PinRemapConfig(MOTOR_GPIO_REMAP, ENABLE);

	/* Enable TIM16, TIM17 clock */
	RCC_APB1PeriphClockCmd(MOTOR_RCC_TIM, ENABLE);

	/* Initializes Use Timer Base */
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Prescaler = MOTOR_TIM_PRESCALE - 1;
	TIM_TimeBaseInitStructure.TIM_Period = MOTOR_TIM_PERIOD - 1;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = 0;

	TIM_TimeBaseInit(MOTORx_TIM, &TIM_TimeBaseInitStructure);

	//Timer OC Configure
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_Pulse = 0;

	TIM_OC1Init(MOTORx_TIM, &TIM_OCInitStructure);
	TIM_OC2Init(MOTORx_TIM, &TIM_OCInitStructure);
	TIM_OC3Init(MOTORx_TIM, &TIM_OCInitStructure);
	TIM_OC4Init(MOTORx_TIM, &TIM_OCInitStructure);

	TIM_Cmd(MOTORx_TIM, ENABLE);

	TIM_OC1PreloadConfig(MOTORx_TIM, TIM_OCPreload_Enable);
	TIM_OC2PreloadConfig(MOTORx_TIM, TIM_OCPreload_Enable);
	TIM_OC3PreloadConfig(MOTORx_TIM, TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(MOTORx_TIM, TIM_OCPreload_Enable);

	TIM_ARRPreloadConfig(MOTORx_TIM, ENABLE);
}

/**
 * @func   MotorControl_ForwardRotating
 * @brief  Setup forward rotation motor x
 * @param  id:id of motor MOTOR_ID_L: Motor left / MOTOR_ID_R: Motor right
 *         dutycycle: level of engine speed (0-100)
 * @retval None
 */
void MotorControl_ForwardRotating(motor_id_e id, uint32_t dutycycle)
{
	uint32_t compare;

	switch (id) {
		case MOTOR_ID_L:
			compare = (dutycycle * MOTOR_TIM_PERIOD) / 100;
			TIM_SetCounter(MOTORx_TIM, 0);
			TIM_SetCounter(MOTORx_TIM, 0);
			TIM_SetCompare3(MOTORx_TIM, 0);
			TIM_SetCompare4(MOTORx_TIM, compare);
			break;

		case MOTOR_ID_R:
			compare = (dutycycle * MOTOR_TIM_PERIOD) / 100;
			TIM_SetCounter(MOTORx_TIM, 0);
			TIM_SetCounter(MOTORx_TIM, 0);
			TIM_SetCompare1(MOTORx_TIM, 0);
			TIM_SetCompare2(MOTORx_TIM, compare);
			break;

		default:
			break;
	}
}

/**
 * @func   MotorControl_ReverseRotating
 * @brief  Setup reverse rotation motor x
 * @param  id:id of motor MOTOR_ID_L: Motor left / MOTOR_ID_R: Motor right
 *         dutycycle: level of engine speed (0-100)
 * @retval None
 */
void MotorControl_ReverseRotating(motor_id_e id, uint32_t dutycycle)
{
	uint32_t compare;

	switch (id) {
		case MOTOR_ID_L:
			compare = (dutycycle * MOTOR_TIM_PERIOD) / 100;
			TIM_SetCounter(MOTORx_TIM, 0);
			TIM_SetCounter(MOTORx_TIM, 0);
			TIM_SetCompare3(MOTORx_TIM, compare);
			TIM_SetCompare4(MOTORx_TIM, 0);
			break;

		case MOTOR_ID_R:
			compare = (dutycycle * MOTOR_TIM_PERIOD) / 100;
			TIM_SetCounter(MOTORx_TIM, 0);
			TIM_SetCounter(MOTORx_TIM, 0);
			TIM_SetCompare1(MOTORx_TIM, compare);
			TIM_SetCompare2(MOTORx_TIM, 0);
			break;

		default:
			break;
	}
}

/* END_FILE */
