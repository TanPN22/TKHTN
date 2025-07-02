#ifndef SERVO_H_INC
#define SERVO_H_INC

#include "main.h"

#define MIN_PULSE_WIDTH 544
#define MAX_PULSE_WIDTH 2400

void Servo_Write(TIM_HandleTypeDef *htim, uint16_t channel, uint8_t angle);
void PWM_Set_Duty(TIM_HandleTypeDef *htim, uint32_t Channel, uint8_t duty);

#endif
