/*******************************************************************************
 *
 * Copyright (c) 2023
 * Lumi, JSC.
 * All Rights Reserved
 *
 *
 * Description:
 *
 * Author: ThaiHM
 *
 * Last Changed By:  $Author: ThaiHM $
 * Revision:         $Revision: 2.0 $
 * Last Changed:     $Date:  28/11/23 $
 *
 ******************************************************************************/
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "button.h"
#include "timer.h"
#include "utilities.h"
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
#define PIN_ACTIVE                     1
#define PIN_UNACTIVE                   0

typedef struct _button_data_ {
    uint32_t time;
    uint32_t holdCnt;
    uint8_t pressCnt;
    uint8_t sold;
	uint8_t index;
	uint8_t mode;
	uint8_t countInactive;
	uint8_t countActive;
	void (*scanBtnFunc)(void *);
} buttondat_t, *buttondat_p;

typedef struct _key_pin_ {
	GPIO_TypeDef *port;
	uint16_t pin;
    uint8_t logicPress;
} buttonPin_t;

/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/
static buttonPin_t pinBtnPad[BUTTON_ID_MAX] = { BUTTON_BACK_PIN, BUTTON_NEXT_PIN, BUTTON_OK_PIN, BUTTON_CANCEL_PIN };
static buttondat_t kpad[BUTTON_ID_MAX];
/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/

/******************************************************************************/
/*                            PRIVATE FUNCTIONS                               */
/******************************************************************************/
static void BTNGPIO_Init(void);
static void ButtonScanTypeEdge(void *arg);
static void ButtonScanEventHandler(void *arg);
static button_event_callback pEventCallbacks[BUTTON_EVENT_MAX];
/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/

/**
 * @func   Button_Init
 * @brief  Initialize module button
 * @param  None
 * @retval None
 */
void
Button_Init(void) {
    // Initialize gpio
	BTNGPIO_Init();
    for (uint8_t i = 0; i < BUTTON_ID_MAX; i++) {
		kpad[i].index = i;
		kpad[i].mode = BUTTON_MODE_LOGIC;
		kpad[i].scanBtnFunc = ButtonScanTypeEdge;
		if (GPIO_ReadInputDataBit(pinBtnPad[i].port, pinBtnPad[i].pin) == pinBtnPad[i].logicPress) {
			kpad[i].sold = PIN_ACTIVE;
		}
	}

    TimerStart("", BUTTON_TIME_SCAN, TIMER_REPEAT_FOREVER, ButtonScanEventHandler, NULL);
}

/**
 * @func   ButtonSetMode
 * @brief  Set mode button
 * @param  None
 * @retval None
 */
void
Button_SetMode(
    uint8_t id,
    uint8_t mode
) {
    if ((id >= BUTTON_ID_MAX) || (mode >= BUTTON_MODE_INVALID)) return;
    kpad[id].mode = mode;
}

/**
 * @func   ButtonGetMode
 * @brief  Get mode button
 * @param  None
 * @retval None
 */
uint8_t
Button_GetMode(
    uint8_t id
) {
    if (id >= BUTTON_ID_MAX) return BUTTON_MODE_INVALID;
    return kpad[id].mode;
}

/**
 * @func   Button_RegisterEventCallback
 * @brief  None
 * @param  None
 * @retval None
 */
void
Button_RegisterEventCallback(
    button_event_e buttonEvent,
    button_event_callback procbuttonEvent
) {
	if ((buttonEvent < BUTTON_EVENT_MAX) && (procbuttonEvent != NULL)) {
        pEventCallbacks[buttonEvent] = procbuttonEvent;
	}
}

/**
 * @func   Button_GetLogicInputPin
 * @brief  Button get logic on input pin
 * @param  id: id of input pin
 * @retval logic pin
 */
uint8_t
Button_GetLogicInputPin(
	uint8_t id
) {
	if (id >= BUTTON_ID_MAX) return 0xFE;

	return GPIO_ReadInputDataBit(pinBtnPad[id].port, pinBtnPad[id].pin);
}

/**
 * @func   ButtonScanTypeEdge
 * @brief  None
 * @param  None
 * @retval None
 */
void
ButtonScanTypeEdge(
    void *arg
) {
	buttondat_p bt = (buttondat_p)arg;
    uint8_t i = bt->index;
    
    if (bt->mode != BUTTON_MODE_IDLE)
    {
		if (GPIO_ReadInputDataBit(pinBtnPad[i].port, pinBtnPad[i].pin) == pinBtnPad[i].logicPress) {
			if (bt->countActive != 0xFFU) {
				bt->countActive++;
			}
		}
		else {
			if (bt->countInactive != 0xFFU) {
				bt->countInactive++;
			}

			bt->countActive = 0;
		}

		if (bt->countInactive >= TIMECNT_UNACTPRESS) {
			if (bt->sold == PIN_ACTIVE) {
				bt->sold = PIN_UNACTIVE;

				if ((pEventCallbacks[BUTTON_EVENT_EDGE] != NULL) && (bt->mode & BUTTON_MODE_EDGE)) {
					pEventCallbacks[BUTTON_EVENT_EDGE](i, BUTTON_EDGE_FALLING);
				}

				bt->time = bt->countInactive;
			} else {
				bt->time++;
				if (bt->time >= TIMECNT_BW2PRESS) {
					if (bt->holdCnt != 0) {
						if ((pEventCallbacks[BUTTON_EVENT_RELEASE] != NULL) && (bt->mode & BUTTON_MODE_LOGIC)) {
							pEventCallbacks[BUTTON_EVENT_RELEASE](i, bt->holdCnt);
						}
					}
					else if ((pEventCallbacks[BUTTON_EVENT_PRESS] != NULL) && (bt->mode & BUTTON_MODE_LOGIC) \
							 && bt->pressCnt != 0)
					{
						pEventCallbacks[BUTTON_EVENT_PRESS](i, bt->pressCnt);
					}

					bt->holdCnt = 0;
					bt->pressCnt = 0;
				}
			}
		}

		if (bt->countActive >= TIMECNT_ACTPRESS) {
			/* reset count inactive */
			bt->countInactive = 0;

			if (bt->sold == PIN_UNACTIVE) {
				bt->sold = PIN_ACTIVE;

				if ((pEventCallbacks[BUTTON_EVENT_EDGE] != NULL) && (bt->mode & BUTTON_MODE_EDGE)) {
					pEventCallbacks[BUTTON_EVENT_EDGE](i, BUTTON_EDGE_RISING);
				}

				bt->time = bt->countActive;
				bt->pressCnt++;
			} else {
				bt->time++;
				if (bt->time >= TIMECNT_IS_HOLD) {
					bt->pressCnt = 0;
					bt->holdCnt = bt->time;
					if ((bt->holdCnt == TIMECNT_IS_HOLD) || (bt->holdCnt == TIMECNT_HOLD1S) || (bt->holdCnt == TIMECNT_HOLD3S) ||
						(bt->holdCnt == TIMECNT_HOLD5S) || (bt->holdCnt == TIMECNT_HOLD10S))
					{
						if (pEventCallbacks[BUTTON_EVENT_HOLD] != NULL && (bt->mode & BUTTON_MODE_LOGIC)) {
							pEventCallbacks[BUTTON_EVENT_HOLD](i, bt->holdCnt);
						}
					}
				}
			}
		}
    }
}

/**
 * @func   ButtonScanEventHandler
 * @brief  Scan key
 * @param  None
 * @retval None
 */
void
ButtonScanEventHandler(
    void *arg
) {
    for (uint8_t i = 0; i < BUTTON_ID_MAX; i++) {
        kpad[i].scanBtnFunc(&kpad[i]);
    }
}

/**
 * @func   BTNGPIO_Init
 * @brief  Initializes module button
 * @param  None
 * @retval None
 */
static void BTNGPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* GPIOB Clocks enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	/* GPIOB Configuration: Channel 1 and 3 as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/* END FILE */
