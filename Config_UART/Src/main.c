#include <stdint.h>
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_usart.h"
#include "misc.h"
#include "utilities.h"
#include "timer.h"
#include "lcd.h"
#include "GUI.h"


void AppInitCommon(void)
{
	TimerInit();		  //Khoi tao timer de su dung ham delay

    LCD_Init();
    LCD_Direction(1);
}

int main(void)
{
	AppInitCommon();
		/* Loop forever */
	while (1)
	{
		LCD_Circle(50, 50, RED, 30, 1);
		LCD_ShowCentredString(BLACK, WHITE, (uint8_t*)"Robi Task 2.2", 16, 1);
		delay_ms(1000);
	}
}
