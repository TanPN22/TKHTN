#include "servo.h"

uint32_t map(uint32_t x, uint32_t in_min, uint32_t in_max, uint32_t out_min, uint32_t out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void Servo_Write(TIM_HandleTypeDef *htim, uint16_t channel, uint8_t angle){
	uint16_t ccr = map(angle, 0, 180, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
	switch (channel) {
		case TIM_CHANNEL_1:
			htim -> Instance -> CCR1 = ccr;
			break;
		case TIM_CHANNEL_2:
			htim -> Instance -> CCR2 = ccr;
			break;
		case TIM_CHANNEL_3:
			htim -> Instance -> CCR3 = ccr;
			break;
		case TIM_CHANNEL_4:
			htim -> Instance -> CCR4 = ccr;
			break;
		default:
			break;
	}
}

//Dutty: 0 -> 100%
void PWM_Set_Duty(TIM_HandleTypeDef *htim, uint32_t Channel, uint8_t duty){
	if (duty >=0 && duty <=100){
		  HAL_TIM_PWM_Start(htim, Channel);
		  htim->Instance -> CCR1 = htim->Instance -> ARR * (duty/100);
	}else;
}
