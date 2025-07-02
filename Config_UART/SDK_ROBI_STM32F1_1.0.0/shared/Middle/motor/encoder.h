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
#ifndef _ENCODER_H_
#define _ENCODER_H_
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include <stdint.h>
#include <stdbool.h>
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/
/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/
extern volatile uint32_t encoderM1_speed;
extern volatile uint32_t encoderM2_speed;
extern bool stateEncoder;
extern int16_t dutyCycle_M1;
extern int16_t dutyCycle_M2;
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
Encoder_Init(void);

#endif
