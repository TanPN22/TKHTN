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
 * Last Changed By:  $Author: HoangNH $
 * Revision:         $Revision: $
 * Last Changed:     $Date: 01/08/2023 $
 *
*******************************************************************************/
#ifndef _MOTOR_H_
#define _MOTOR_H_
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include <stdint.h>
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
/*! @brief Motor Pins */
#define MOTOR_RCC_GPIO					RCC_APB2Periph_GPIOB
#define MOTOR_RCC_TIM					RCC_APB1Periph_TIM4
#define MOTOR_GPIO_REMAP				GPIO_Remap_TIM4
#define MOTOR_GPIO_PORT               	GPIOB
#define MOTORx_TIM						TIM4

/*! @brief Motor Right Pins */
#define MOTOR_R1_GPIO_PIN				GPIO_Pin_6
#define MOTOR_R1_PinSource				GPIO_PinSource6

#define MOTOR_R2_GPIO_PIN				GPIO_Pin_7
#define MOTOR_R2_PinSource				GPIO_PinSource7

/*! @brief Motor Left Pins */
#define MOTOR_L1_GPIO_PIN				GPIO_Pin_8
#define MOTOR_L1_PinSource				GPIO_PinSource8

#define MOTOR_L2_GPIO_PIN				GPIO_Pin_9
#define MOTOR_L2_PinSource				GPIO_PinSource9

/*! @brief Motor ID */
typedef enum _MOTOR_ID_ {
	MOTOR_ID_L,
	MOTOR_ID_R,
	MOTOR_ID_MAX
} motor_id_e;

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
 * @func   MotorControl_Init
 * @brief  Initializes module motor
 * @param  None
 * @retval None
 */
void MotorControl_Init(void);

/**
 * @func   MotorControl_ForwardRotating
 * @brief  Setup forward rotation motor x
 * @param  id:id of motor MOTOR_ID_L: Motor left / MOTOR_ID_R: Motor right
 *         dutycycle: level of engine speed (0-100)
 * @retval None
 */
void MotorControl_ForwardRotating(motor_id_e id, uint32_t dutycycle);

/**
 * @func   MotorControl_ReverseRotating
 * @brief  Setup reverse rotation motor x
 * @param  id:id of motor MOTOR_ID_L: Motor left / MOTOR_ID_R: Motor right
 *         dutycycle: level of engine speed (0-100)
 * @retval None
 */
void MotorControl_ReverseRotating(motor_id_e id, uint32_t dutycycle);

#endif
