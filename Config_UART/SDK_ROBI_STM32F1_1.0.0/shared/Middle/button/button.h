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
#ifndef _BUTTON_H_
#define _BUTTON_H_
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include <stdint.h>
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
/*! @brief Pins input control direction */
#define BUTTON_BACK_PIN                  { GPIOB, GPIO_Pin_2,  0 }
#define BUTTON_NEXT_PIN                  { GPIOB, GPIO_Pin_10, 0 }
#define BUTTON_OK_PIN                 	 { GPIOB, GPIO_Pin_1,  0 }
#define BUTTON_CANCEL_PIN                { GPIOB, GPIO_Pin_0,  0 }

#define BUTTON_TIME_SCAN                 5u // 5ms

#define BUTTON_TIME_UNACTPRESS			 50u
#define BUTTON_TIME_ACTPRESS			 50u
#define BUTTON_TIME_BW2PRESS             200u
#define BUTTON_TIME_IS_HOLD              600u
#define BUTTON_TIME_HOLD1S               1000u
#define BUTTON_TIME_HOLD3S               3000u
#define BUTTON_TIME_HOLD5S               5000u
#define BUTTON_TIME_HOLD10S              10000u

#define TIMECNT_UNACTPRESS               (BUTTON_TIME_UNACTPRESS / BUTTON_TIME_SCAN)
#define TIMECNT_ACTPRESS           	     (BUTTON_TIME_ACTPRESS   / BUTTON_TIME_SCAN)
#define TIMECNT_BW2PRESS                 (BUTTON_TIME_BW2PRESS   / BUTTON_TIME_SCAN)
#define TIMECNT_IS_HOLD                  (BUTTON_TIME_IS_HOLD    / BUTTON_TIME_SCAN)
#define TIMECNT_HOLD1S                   (BUTTON_TIME_HOLD1S     / BUTTON_TIME_SCAN)
#define TIMECNT_HOLD3S                   (BUTTON_TIME_HOLD3S     / BUTTON_TIME_SCAN)
#define TIMECNT_HOLD5S                   (BUTTON_TIME_HOLD5S     / BUTTON_TIME_SCAN)
#define TIMECNT_HOLD10S                  (BUTTON_TIME_HOLD10S    / BUTTON_TIME_SCAN)

#define BUTTON_EDGE_RISING               0
#define BUTTON_EDGE_FALLING              1

typedef enum {
	BUTTON_BACK_ID,
	BUTTON_NEXT_ID,
	BUTTON_OK_ID,
	BUTTON_CANCEL_ID,
	BUTTON_ID_MAX
} button_id_e;

typedef enum {
    BUTTON_EVENT_EDGE,
	BUTTON_EVENT_PRESS,
    BUTTON_EVENT_HOLD,
    BUTTON_EVENT_RELEASE,
    BUTTON_EVENT_MAX
} button_event_e;

typedef enum {
	BUTTON_MODE_IDLE,
    BUTTON_MODE_LOGIC,
    BUTTON_MODE_EDGE,
	BUTTON_MODE_INVALID
} button_mode_e;

typedef void (* button_event_callback)(uint8_t id, uint16_t time);
/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/
/******************************************************************************/
/*                              EXPORTED DATA                                 */
/******************************************************************************/

/******************************************************************************/
/*                            PRIVATE FUNCTIONS                               */
/******************************************************************************/

/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
///******************************************************************************/
//void EXTI0_IRQHandler(void);
/**
 * @func   Button_Init
 * @brief  Initialize module button
 * @param  None
 * @retval None
 */
void
Button_Init(void);

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
);

/**
 * @func   ButtonSetMode
 * @brief  Set mode key
 * @param  None
 * @retval None
 */
void
Button_SetMode(
    uint8_t id,
    uint8_t mode
);

/**
 * @func   ButtonGetMode
 * @brief  Get mode key
 * @param  None
 * @retval None
 */
uint8_t
Button_GetMode(
    uint8_t id
);

/**
 * @func   Button_GetLogicInputPin
 * @brief  Button get logic on input pin
 * @param  id: id of input pin
 * @retval logic pin
 */
uint8_t
Button_GetLogicInputPin(
	uint8_t id
);

#endif

/* END FILE */
