/*******************************************************************************
 *
 * Copyright (c) 2019
 * Lumi, JSC.
 * All Rights Reserved
 *
 *
 * Description: Include file for application
 *
 * Author: HoangNH
 *
 * Last Changed By:  $Author: HoangNH $
 * Revision:         $Revision: 1.1 $
 * Last Changed:     $Date: 2/8/2023 $
 *
 ******************************************************************************/
#ifndef _OPTICAL_SENSOR_H_
#define _OPTICAL_SENSOR_H_
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include <stdint.h>
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/

/*@ !brief ADC pins */
#define IRx_SENSOR_RCC_GPIO         RCC_APB2Periph_GPIOA
#define IRx_SENSOR_RCC_ADC          RCC_APB2Periph_ADC1
#define IRx_SENSOR_RCC_DMA          RCC_AHBPeriph_DMA1
#define IRx_SENSOR_PORT				GPIOA

#define IR1_SENSOR_PIN				GPIO_Pin_0
#define IR2_SENSOR_PIN				GPIO_Pin_1
#define IR3_SENSOR_PIN				GPIO_Pin_2
#define IR4_SENSOR_PIN				GPIO_Pin_3
#define IR5_SENSOR_PIN				GPIO_Pin_4

#define IRx_SENSOR					ADC1

#define IR_SENSOR_MAX	 			5
#define AVERAGE_ADC					200

#define IRSENSOR_NUM_SAMPLING		200
#define ADC1_DR_Address    			((uint32_t)0x4001244C)

/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/
extern uint16_t AdcValuesThreshold[IR_SENSOR_MAX];
/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/
/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/

/**
 * @func   OpticalSensor_Init
 * @brief  Initializes module Reflective Optical Sensor
 * @param  None
 * @retval None
 */
void
OpticalSensor_Init(void);

/**
 * @func   OpticalSensor_MeasureUseDMAMode
 * @brief  Measure value ADC in mode DMA
 * @param  irSensID: 0,1,2,3,4.
 * @retval Value of ADC
 */
uint16_t
OpticalSensor_MeasureUseDMAMode(uint8_t irSensID);

/**
 * @func   SamplingWhite
 * @brief  Sampling White line
 * @param  None
 * @retval None
 */
void SampleWhite(void);

/**
 * @func   SamplingBlack
 * @brief  Sampling Black Line
 * @param  None
 * @retval None
 */
void SampleBlack(void);

/**
 * @func   Calculate_AdcThreshold
 * @brief  Calculate Threshold for per IR sensor
 * @param  None
 * @retval Threshold of per Adc Values
 */
void CalculateAdcThreshold(void);

#endif

/* END FILE */
