/*******************************************************************************
 *
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 *
 * Description:
 *
 * Author: ThangDH
 *
 * Last Changed By:  $Author: thangdh $
 * Revision:         $Revision: 1.1 $
 * Last Changed:     $Date: 17/08/17 09:00 $
 *
 ******************************************************************************/
#ifndef _UTILITIES_H_
#define _UTILITIES_H_
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include <stdint.h>
/******************************************************************************/
/*                     EXPORTED TYPES and DEFINITIONS                         */
/******************************************************************************/
#define NOTFOUND                    0xFFFF

#ifndef NULL
#define NULL                        0
#endif // NULL
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
/******************************************************************************/

/**
 * @func   searchArrayInArray
 * @brief  None
 * @param  None
 * @retval offset ArrKey match in ArrData OR NOT_FOUND
 */
uint16_t
searchArrayInArray(
    const uint8_t *pArrKey,
	uint8_t keyLength,
    const uint8_t *pArrData,
	uint16_t dataLength
);

/**
 * @func   hex2Char 
 * @brief  None
 * @param  [in] 
 * @retval char
 */
uint8_t
hex2Char(
	uint8_t byHex
); 

/**
 * @func   String2Uint 
 * @brief  None
 * @param  [in] 
 * @retval 
 */
uint32_t
String2Uint(
	uint8_t *pChar,
	uint8_t offset,
	uint8_t length
);

void
memcpyl(
    uint8_t *dst,
	uint8_t *src,
	uint16_t size
);

void
memsetl(
	uint8_t *dst,
	uint8_t value,
	uint16_t size
);

/**
 * @func   valInRange 
 * @brief  None
 * @param  None
 * @retval None
 */
uint8_t
valInRange(
	uint32_t val,
	uint32_t rmin,
	uint32_t rmax
);

uint32_t
dwCalculatorTime(
	uint32_t dwTimeInit,
	uint32_t dwTimeCurrent
);

void
delay_ms(
    uint32_t ms
);

/**
 * @func	mypow
 * @brief	get the nth power of m (internal call)
 * @param	m:the multiplier
			n:the power
 * @retval	the nth power of m
*/
uint32_t mypow(
		uint8_t m,
		uint8_t n
);
#endif /* END FILE */
