/*******************************************************************************
 *
 * Copyright (c) 2023
 * Lumi, JSC.
 * All Rights Reserved
 *
 *
 * Description:
 *
 * Author: TanPN
 *
 * Last Changed By:  $Author: TanPN $
 * Revision:         $Revision: 2.0 $
 * Last Changed:     $Date:  28/11/23 $
 *
 ******************************************************************************/
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include "buzzer.h"
#include "timer.h"
#include "melody.h"
#include "utilities.h"

#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
#define BUZZ_TIMER_PRESCALE    8
#define BUZZ_TIMER_PERIOD      500
/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/
static uint8_t bBuzzInit = 0;

/******************************************************************************/
/*                            PRIVATE FUNCTIONS                               */
/******************************************************************************/
static void BUZZTIM_Init(void);
/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/

/**
 * @func   BuzzerControl_Init
 * @brief  Initialize module buzzer
 * @param  None
 * @retval None
 */
void
BuzzerControl_Init(void) {
	BUZZTIM_Init();
	bBuzzInit = 1;
	BuzzerControl_SetDutyCycle(0);
}

/**
 * @func   BuzzerControl_SetMelody
 * @brief  Buzzer set melody
 * @param  pListTone
 * @retval None
 */
void 
BuzzerControl_SetFreqMelody(
		uint32_t freq
) {
	uint64_t tempFreq = freq;
	if(freq == 0) tempFreq = 1;

	uint64_t tempNewValue = (uint64_t) (1000000) / tempFreq;

	// setting new value
	TIM2 ->ARR = (uint32_t)tempNewValue;
	BuzzerControl_SetDutyCycle(NOTE_VOLUME);
}

/**
 * @func   BuzzerControl_SetDutyCycle
 * @brief  Set duty cycle PWM
 * @param  dutycycle
 * dutycycle: The length of PWM signal. Must be in range (0->100)
 * @retval None
 */
void
BuzzerControl_SetDutyCycle(
    uint8_t dutycycle
) {
	uint32_t value;

	if (dutycycle > 100) return;

	value = (dutycycle * TIM2->ARR) / 100;

	TIM2->CCR4 = value;
}

/**
 * @func   TIMPWM_InitChannel
 * @brief  Initialize peripheral timer used to control buzzer
 * @param  None
 * @retval None
 */
static
void BUZZTIM_Init(void)
{
    GPIO_InitTypeDef 			GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef 	TimeBaseStructure;
    TIM_OCInitTypeDef 			TIM_OCInitStructure;

    /* GPIO Peripheral clock enable */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

//    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
    GPIO_PinRemapConfig(GPIO_FullRemap_TIM2, ENABLE);


    /* Configure pin in output push-pull mode */
    GPIO_InitStructure.GPIO_Pin = BUZZER_GPIO_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(BUZZER_GPIO_PORT, &GPIO_InitStructure);


    /* Time base configuration */
	TimeBaseStructure.TIM_Prescaler = BUZZ_TIMER_PRESCALE - 1 ;
	TimeBaseStructure.TIM_Period = BUZZ_TIMER_PERIOD - 1;
	TimeBaseStructure.TIM_ClockDivision = 0;
	TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &TimeBaseStructure);

    /* Output Compare Timing Mode configuration: Channel 4 */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Set;

    TIM_OC4Init(TIM2, &TIM_OCInitStructure);

    TIM_CtrlPWMOutputs(TIM2, ENABLE);

    TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);

    TIM_ARRPreloadConfig(TIM2, ENABLE);

	/* TIM2 enable counter */
	TIM_Cmd(TIM2, ENABLE);
}

/* END_FILE */

