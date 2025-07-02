/*******************************************************************************
 *
 * Copyright (c) 2023
 * Lumi, JSC.
 * All Rights Reserved
 *
 * Description: Include file for application
 *
 * Author: ThaiHM
 *
 * Last Changed By:  $Author: ThaiHM $
 * Revision:         $Revision: $
 * Last Changed:     $Date: 30/11/2023 $
 *
*******************************************************************************/
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include "stm32f10x_exti.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "misc.h"
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include "encoder.h"
#include "timer.h"
#include "motor.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
/*! @brief Encoder parameters */
#define ENCODER_PULSE_ROUND		210   		// pulse
#define SAMPLE_TIME				1

/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/
/*! @brief Encoder values */
char State[16] = {0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0};
char EncoderM1_Val = 0;
char EncoderM2_Val = 0;

volatile uint32_t encoderM1_temp;
volatile uint32_t encoderM2_temp;

volatile uint32_t SpeedM1_1ms;
volatile uint32_t SpeedM2_1ms;
static uint16_t count = 0;

volatile uint32_t encoderM1_speed = 0;
volatile uint32_t encoderM2_speed = 0;

bool stateEncoder = false;

static SSwTimer idTimerScanSpeed = NO_TIMER;
/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/
/******************************************************************************/
/*                            PRIVATE FUNCTIONS                               */
/******************************************************************************/
static void EncoderGPIO_Config(void);
int16_t binaryToDecimal(int16_t binaryNumber);
static void ScanTimer(void);
/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/

/**
 * @func   Encoder_Init
 * @brief  Initializes encoder to read speed of motor
 * @param  None
 * @retval None
 */
void
Encoder_Init(void)
{
	EncoderGPIO_Config();
	ScanTimer();
}

/**
 * @func   Encoder1GPIO_Config
 * @brief  Configure GPIO for encoder left
 * @param  None
 * @retval None
 */
static
void EncoderGPIO_Config(void)
{
	GPIO_InitTypeDef 			GPIO_InitStructure;
	EXTI_InitTypeDef			EXTI_InitStructure;
	NVIC_InitTypeDef			NVIC_InitStructure;

	/* Enable clock GPIOA and GPIOB */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);

	/* Initializes GPIO Use Input */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_15;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* Enable AFIO clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

	/* Connect EXTI Line to Encoder pin */
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource15);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource3);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource4);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource5);

	/* Configure EXTI Line */
	EXTI_InitStructure.EXTI_Line =  EXTI_Line3 | EXTI_Line5;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	/* Configure NVIC for Encoder Interface Update Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn | EXTI9_5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/**
 * @func   binaryToDecimal
 * @brief  Convert Binary To Decimal
 * @param  Binary Number
 * @retval Decimal Number
 */
int16_t binaryToDecimal(int16_t binaryNumber)
{
    int16_t decimalNumber = 0, i = 0, remainder;

    while (binaryNumber != 0) {
        remainder = binaryNumber % 10;
        binaryNumber /= 10;
        decimalNumber += remainder * pow(2, i);
        ++i;
    }

    return decimalNumber;
}

/**
 * @func   EXTI3_IRQHandler
 * @brief  Handler interrupt of encoder M1 for count pulse
 * @param  None
 * @retval None
 */
void EXTI3_IRQHandler(void)
{
	/* Count Pulse Encoder M1 */
	char EncoderM1_pinA = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_3);
	char EncoderM1_pinB = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_15);

	/* Handler EXTI_Line3 to PB3 */
	if(EXTI_GetFlagStatus(EXTI_Line3) == SET) {
		/* Pulse Noise Filtering */
		EncoderM1_Val   = EncoderM1_Val << 2;
		EncoderM1_Val   = (( EncoderM1_pinA | EncoderM1_pinB) | EncoderM1_Val ) & 0x0f;
		encoderM1_temp += State[binaryToDecimal(EncoderM1_Val)];

		EXTI_ClearITPendingBit(EXTI_Line3);
	}
}

/**
 * @func   EXTI9_5_IRQHandler
 * @brief  Handler interrupt of encoder M2 for count pulse
 * @param  None
 * @retval None
 */
void EXTI9_5_IRQHandler(void)
{
	/* Count Pulse Encoder M2 */
	char EncoderM2_pinA = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5);
	char EncoderM2_pinB = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_4);

	/* Handler EXTI_Line5 to PB5 */
	if(EXTI_GetFlagStatus(EXTI_Line5) == SET) {
		/* Pulse Noise Filtering */
		EncoderM2_Val   = EncoderM2_Val << 2;
		EncoderM2_Val   = (( EncoderM2_pinA | EncoderM2_pinB) | EncoderM2_Val ) & 0x0f;
		encoderM2_temp += State[binaryToDecimal(EncoderM2_Val)];

		EXTI_ClearITPendingBit(EXTI_Line5);
	}
}

/**
 * @func   CalculateSpeed
 * @brief  Calculate speed encoder
 * @param  None
 * @retval None
 */
static
void CalculateSpeed(void *arg)
{
	/* Calculate speed encoder in 1ms */
	if (stateEncoder == false) {
		SpeedM1_1ms += (uint32_t)((encoderM1_temp * 1000 * 60) / ENCODER_PULSE_ROUND);	// rpm
		SpeedM2_1ms += (uint32_t)((encoderM2_temp * 1000 * 60) / ENCODER_PULSE_ROUND);	// rpm
		count++;

		/* Calculate speed encoder in 5ms */
		if(count >= 5) {
			stateEncoder = true;
			encoderM1_speed = (uint32_t)(SpeedM1_1ms / count);	// rpm
			encoderM2_speed = (uint32_t)(SpeedM2_1ms / count);	// rpm

			/* Reset Count Pulse, Speed, Count */
			encoderM1_temp = 0;
			encoderM2_temp = 0;
			SpeedM1_1ms = 0;
			SpeedM2_1ms = 0;
			count = 0;
		}
	}
}

/**
 * @func   ScanTimer
 * @brief  Scan timer for calculate speed
 * @param  None
 * @retval None
 */
static
void ScanTimer(void)
{
	if(idTimerScanSpeed != NO_TIMER) {
		TimerStop(idTimerScanSpeed);
		idTimerScanSpeed = NO_TIMER;
	}

	idTimerScanSpeed = TimerStart("CalculateSpeed", 	\
								  SAMPLE_TIME, 			\
								  TIMER_REPEAT_FOREVER, \
							 	  CalculateSpeed,       \
								  NULL);
}

/* END_FILE */
