/*******************************************************************************
 *
 * Copyright (c) 2023
 * Lumi, JSC.
 * All Rights Reserved
 *
 * Description: Include file for application
 *
 * Author: TanPN
 *
 * Last Changed By:  $Author: TanPN $
 * Revision:         $Revision: $
 * Last Changed:     $Date: 22/11/2023 $
 *
*******************************************************************************/
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include "pid.h"
#include "led.h"
#include "motor.h"
#include "utilities.h"
#include "optical_sensor.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/
/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/
static void PID_Control(void);
/******************************************************************************/
/*                            PRIVATE FUNCTIONS                               */
/******************************************************************************/

/*@brief ADC Threshold values */
uint16_t AdcValuesThreshold[IR_SENSOR_MAX] = {2500,2500,3000,2500,2500};

/*@brief PID values */
int P, I, D;
int previousError, error;
float PIDvalue = 0;
float Kp = 0.0;
float Ki = 0.0;
float Kd = 0.0;
int lfspeed = 40;
int lsp, rsp;
int lastCheckInline;
int count = 0;
/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/

/**
 * @func   LineFollow
 * @brief
 * @param  None
 * @retval None
 */
void LineFollow(void)
{
	/* IR sensor is in the leftmost position */
    if (OpticalSensor_MeasureUseDMAMode(0) < AdcValuesThreshold[0] &&
		OpticalSensor_MeasureUseDMAMode(4) > AdcValuesThreshold[4])
    {
    	/* Turn left */
		MotorControl_ForwardRotating(MOTOR_ID_R, 25);	// if max speed = 60
		MotorControl_ReverseRotating(MOTOR_ID_L, 15);
//		MotorControl_ForwardRotating(MOTOR_ID_R, 30);	// if max speed = 100
//		MotorControl_ReverseRotating(MOTOR_ID_L, 15);

    }

    /* IR sensor is in the rightmost position */
    else if (OpticalSensor_MeasureUseDMAMode(0) > AdcValuesThreshold[0] &&
    		 OpticalSensor_MeasureUseDMAMode(4) < AdcValuesThreshold[4])
    {
    	/* Turn right */
		MotorControl_ReverseRotating(MOTOR_ID_R, 15);	// if max speed = 60
		MotorControl_ForwardRotating(MOTOR_ID_L, 25);
//		MotorControl_ReverseRotating(MOTOR_ID_R, 15);	// if max speed = 100
//		MotorControl_ForwardRotating(MOTOR_ID_L, 30);
    }

    /* IR sensor is in the center position */
    else if (OpticalSensor_MeasureUseDMAMode(2) < AdcValuesThreshold[2])
    {
    	/* Calculate Kp, Ki, Kd used to Follow Line */
		Kp = 0.000085 * (1500 - OpticalSensor_MeasureUseDMAMode(2));
		Kd = 0.0075 * (1500 - OpticalSensor_MeasureUseDMAMode(2));;
		Ki = 0.000015;
		PID_Control();
    }
}

/**
 * @func   PID_Control
 * @brief
 * @param  None
 * @retval None
 */
static void PID_Control(void)
{
	/* Calculate error */
	int error = (OpticalSensor_MeasureUseDMAMode(1) - OpticalSensor_MeasureUseDMAMode(3))/8;

	P =  error;
	I += error;
	D =  error - previousError;

	PIDvalue = ((Kp * P) + (Ki * I) + (Kd * D));
	previousError = error;

	lsp = lfspeed - PIDvalue;
	rsp = lfspeed + PIDvalue;

	if (lsp > 65) 	lsp = 65;

	if (lsp < 0) 	lsp = 0;

	if (rsp > 65) 	rsp = 65;

	if (rsp < 0) 	rsp = 0;


	MotorControl_ForwardRotating(MOTOR_ID_R, rsp);
	MotorControl_ForwardRotating(MOTOR_ID_L, lsp);
}

/**
 * @func   Check_InLine
 * @brief  Check IR sensor In Line or Out Line
 * @param  None
 * @retval Number IR sensor In Line and Out Line
 */
uint8_t Check_InLine(void)
{
	for(uint8_t i = 0; i < IR_SENSOR_MAX; i++){
		if(OpticalSensor_MeasureUseDMAMode(i) > AdcValuesThreshold[i]) {
			count++;
		}
		else {
			count--;
		}
	}

	if (count == 5){
		count = 0;
		return 1;
	}else if (count == -5){
		count = 0;
		return 2;
	}
	else {
		count = 0;
		return 0;
	}
}

/**
 * @func   MotorControl_PID
 * @brief  Control motor in line, all black or all white
 * @param  None
 * @retval None
 */
void MotorControl_PID(void)
{
	if (Check_InLine() == 1) {
//		MotorControl_ReverseRotating(MOTOR_ID_L, 20);
//		MotorControl_ReverseRotating(MOTOR_ID_R, 20);
		Toggle_LedYellow();
	} else if (Check_InLine() == 2) {
		MotorControl_ForwardRotating(MOTOR_ID_L, 0);
		MotorControl_ForwardRotating(MOTOR_ID_R, 0);
		for (int i = 0; i < 5; i++) {
			Toggle_LedRed();
			delay_ms(250);
		}
	} else {
		LineFollow();
	}

	lastCheckInline = Check_InLine();
}
