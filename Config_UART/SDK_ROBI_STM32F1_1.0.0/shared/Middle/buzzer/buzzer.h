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
#ifndef _BUZZER_H_
#define _BUZZER_H_
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include <stdint.h>
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
#define BUZZER_GPIO_CLK         RCC_APB2Periph_GPIOB
#define BUZZER_GPIO_PORT		GPIOB
#define BUZZER_GPIO_PIN			GPIO_Pin_11
#define BUZZER_GPIO_SOURCE      GPIO_PinSource11
#define BUZZER_GPIO_REMAP       GPIO_FullRemap_TIM2

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
BuzzerControl_Init(void);

/**
 * @func   BuzzerControl_SetFreqMelody
 * @brief  Buzzer set frequency of melody
 * @param  freq: frequency of sound.(usually in range (100 -> 800)
 * 		   user can use this function to create music not like do re mi fa sol la si.
 * @retval None
 */
void 
BuzzerControl_SetFreqMelody(
    uint32_t freq
);

/**
 * @func   BuzzerControl_SetDutyCycle
 * @brief  Set duty cycle Buzzer
 * @param  dutycycle: loudness level of buzzer (0-100)
 * @retval None
 */
void
BuzzerControl_SetDutyCycle(
    uint8_t dutycycle
);

#endif

/* END FILE */
