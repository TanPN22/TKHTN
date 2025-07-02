/*******************************************************************************
 *
 * Copyright (c) 2020
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
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include "stm32f10x_adc.h"
#include "stm32f10x_dma.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "optical_sensor.h"
#include "motor.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/
static __IO uint16_t ADCConvertedValue[IR_SENSOR_MAX];

uint16_t AdcValues_White[IR_SENSOR_MAX];
uint16_t AdcValues_Black[IR_SENSOR_MAX];
/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/
/******************************************************************************/
/*                            PRIVATE FUNCTIONS                               */
/******************************************************************************/
/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/

/**
 * @func   OpticalSensor_Init
 * @brief  Initializes module light sensor
 * @param  None
 * @retval None
 */
void
OpticalSensor_Init(void)
{
	ADC_InitTypeDef 	ADC_InitStructure;
	GPIO_InitTypeDef 	GPIO_InitStructure;
	DMA_InitTypeDef 	DMA_InitStructure;

	/* ADCCLK = PCLK2/4 */
	RCC_ADCCLKConfig(RCC_PCLK2_Div4);

	/* Enable peripheral clocks ***********************************************/
	RCC_APB2PeriphClockCmd(IRx_SENSOR_RCC_GPIO, ENABLE);
	RCC_APB2PeriphClockCmd(IRx_SENSOR_RCC_ADC, ENABLE);
	RCC_AHBPeriphClockCmd(IRx_SENSOR_RCC_DMA, ENABLE);

	GPIO_InitStructure.GPIO_Pin = IR1_SENSOR_PIN | IR2_SENSOR_PIN |
								  IR3_SENSOR_PIN | IR4_SENSOR_PIN |
								  IR5_SENSOR_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(IRx_SENSOR_PORT, &GPIO_InitStructure);

	/* ADC De-initialization ***************************************************/
	ADC_DeInit(IRx_SENSOR);

	/* Initialize ADC structure */
	ADC_StructInit(&ADC_InitStructure);

	/* Configure the ADC1 in continuous mode with a resolution equal to 12 bits  */
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 5;
	ADC_Init(IRx_SENSOR, &ADC_InitStructure);

	/* ADC1 regular channels configuration */
	ADC_RegularChannelConfig(IRx_SENSOR, ADC_Channel_0, 1, ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(IRx_SENSOR, ADC_Channel_1, 2, ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(IRx_SENSOR, ADC_Channel_2, 3, ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(IRx_SENSOR, ADC_Channel_3, 4, ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(IRx_SENSOR, ADC_Channel_4, 5, ADC_SampleTime_239Cycles5);

	/* Enable the ADC peripheral */
	ADC_Cmd(IRx_SENSOR, ENABLE);

	/* Enable ADC_DMA */
	ADC_DMACmd(IRx_SENSOR, ENABLE);

	/* Enable ADC1 reset calibration register */
	ADC_ResetCalibration(IRx_SENSOR);

	/* Check the end of ADC1 reset calibration register */
	while(ADC_GetResetCalibrationStatus(IRx_SENSOR));

	/* Start ADC1 calibration */
	ADC_StartCalibration(IRx_SENSOR);

	/* Check the end of ADC1 calibration */
	while(ADC_GetCalibrationStatus(IRx_SENSOR));

	/* Start ADC1 Software Conversion */
	ADC_SoftwareStartConvCmd(IRx_SENSOR, ENABLE);

	/* DMA1 channel1 configuration ----------------------------------------------*/
	DMA_DeInit(DMA1_Channel1);
	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&ADCConvertedValue;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = 5;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);

	/* Enable DMA1 channel1 */
	DMA_Cmd(DMA1_Channel1, ENABLE);
}

/**
 * @func   OpticalSensor_MeasureUseDMAMode
 * @brief  Measure value ADC in mode DMA
 * @param  irSensID: 0,1,2,3,4.
 * @retval Value of ADC
 */
uint16_t
OpticalSensor_MeasureUseDMAMode(uint8_t irSensID)
{
	if (irSensID >= (sizeof(ADCConvertedValue) / sizeof(ADCConvertedValue[0])))
		return 0;

	return ADCConvertedValue[irSensID];
}


/**
 * @func   SamplingWhite
 * @brief  Sampling White line
 * @param  None
 * @retval None
 */
void SampleWhite(void)
{
	uint32_t sample[IR_SENSOR_MAX] = {0};

	for(uint8_t i = 0; i < IR_SENSOR_MAX; i++)
	{
		for(uint8_t j = 0; j < IRSENSOR_NUM_SAMPLING; j++)
		{
			sample[i] += OpticalSensor_MeasureUseDMAMode(i);

		}

		AdcValues_White[i] = sample[i] / IRSENSOR_NUM_SAMPLING;
	}
}

/**
 * @func   SamplingBlack
 * @brief  Sampling Black Line
 * @param  None
 * @retval None
 */
void SampleBlack(void)
{
	uint32_t sample[IR_SENSOR_MAX] = {0};

	for(uint8_t i = 0; i < IR_SENSOR_MAX; i++)
	{
		for(uint8_t j = 0; j < IRSENSOR_NUM_SAMPLING; j++)
		{
			sample[i] += OpticalSensor_MeasureUseDMAMode(i);

		}

		AdcValues_Black[i] = sample[i] / IRSENSOR_NUM_SAMPLING;
	}
}

/**
 * @func   Calculate_AdcThreshold
 * @brief
 * @param  None
 * @retval Threshold of per Adc Values
 */
void CalculateAdcThreshold(void)
{
	for(uint8_t i = 0; i < IR_SENSOR_MAX; i++) {
		AdcValuesThreshold[i] = ((AdcValues_White[i] + AdcValues_Black[i]) / 2);
	}
}

/* END FILE */
