/*******************************************************************************
 *
 * Copyright (c) 2023
 * Lumi, JSC.
 * All Rights Reserved
 *
 *
 * Description:
 *
 * Author: TanPN
 *
 * Last Changed By:  $Author: TanPN $
 * Revision:         $Revision: 2.0 $
 * Last Changed:     $Date:  28/11/23 $
 *
 ******************************************************************************/
/*Use this following code to play a basic music.
//	 TIM2 -> CR1 |= TIM_CR1_CEN;
//	 TIM2 ->CCER |= TIM_CCER_CC4E;
//
//	 int melodyCount = sizeof(melodySizes)/ sizeof(uint32_t);
//
//	 for(int melodyIndex = 0; melodyIndex < melodyCount; melodyIndex++)
//	  {
//		  for(int noteIndex = 0; noteIndex < melodySizes[melodyIndex]; noteIndex++)
//	  	  {
//			  BuzzerControl_SetFreqMelody(melody[melodyIndex][noteIndex]);
//		  	  delay_ms(noteDurations[melodyIndex][noteIndex] * melodySlowfactor[melodyIndex]);
//	  	  }
//	  }
 */
/******************************************************************************/
/*                              INCLUDE FILES                                 */
/******************************************************************************/
#include "melody.h"
#include "stdint.h"
#include "buzzer.h"
#include "utilities.h"

/******************************************************************************/
/*                              PRIVATE DATA                                  */
/******************************************************************************/
const uint32_t marioMelody[] = {
		  NOTE_E7, NOTE_E7, 0, NOTE_E7,
		  0, NOTE_C7, NOTE_E7, 0,
		  NOTE_G7, 0, 0,  0,
		  NOTE_G6, 0, 0, 0,

		  NOTE_C7, 0, 0, NOTE_G6,
		  0, 0, NOTE_E6, 0,
		  0, NOTE_A6, 0, NOTE_B6,
		  0, NOTE_AS6, NOTE_A6, 0,

		  NOTE_G6, NOTE_E7, NOTE_G7,
		  NOTE_A7, 0, NOTE_F7, NOTE_G7,
		  0, NOTE_E7, 0, NOTE_C7,
		  NOTE_D7, NOTE_B6, 0, 0,

		  NOTE_C7, 0, 0, NOTE_G6,
		  0, 0, NOTE_E6, 0,
		  0, NOTE_A6, 0, NOTE_B6,
		  0, NOTE_AS6, NOTE_A6, 0,

		  NOTE_G6, NOTE_E7, NOTE_G7,
		  NOTE_A7, 0, NOTE_F7, NOTE_G7,
		  0, NOTE_E7, 0, NOTE_C7,
		  NOTE_D7, NOTE_B6, 0, 0
};

const uint32_t secondMelody[] = {
		  NOTE_D4, NOTE_G4, NOTE_FS4, NOTE_A4,
		  NOTE_G4, NOTE_C5, NOTE_AS4, NOTE_A4,
		  NOTE_FS4, NOTE_G4, NOTE_A4, NOTE_FS4, NOTE_DS4, NOTE_D4,
		  NOTE_C4, NOTE_D4,0,

		  NOTE_D4, NOTE_G4, NOTE_FS4, NOTE_A4,
		  NOTE_G4, NOTE_C5, NOTE_D5, NOTE_C5, NOTE_AS4, NOTE_C5, NOTE_AS4, NOTE_A4,      //29               //8
		  NOTE_FS4, NOTE_G4, NOTE_A4, NOTE_FS4, NOTE_DS4, NOTE_D4,
		  NOTE_C4, NOTE_D4,0,

		  NOTE_D4, NOTE_FS4, NOTE_G4, NOTE_A4, NOTE_DS5, NOTE_D5,
		  NOTE_C5, NOTE_AS4, NOTE_A4, NOTE_C5,
		  NOTE_C4, NOTE_D4, NOTE_DS4, NOTE_FS4, NOTE_D5, NOTE_C5,
		  NOTE_AS4, NOTE_A4, NOTE_C5, NOTE_AS4,             //58

		  NOTE_D4, NOTE_FS4, NOTE_G4, NOTE_A4, NOTE_DS5, NOTE_D5,
		  NOTE_C5, NOTE_D5, NOTE_C5, NOTE_AS4, NOTE_C5, NOTE_AS4, NOTE_A4, NOTE_C5, NOTE_G4,
		  NOTE_A4, 0, NOTE_AS4, NOTE_A4, 0, NOTE_G4,
		  NOTE_G4, NOTE_A4, NOTE_G4, NOTE_FS4, 0,

		  NOTE_C4, NOTE_D4, NOTE_G4, NOTE_FS4, NOTE_DS4,
		  NOTE_C4, NOTE_D4, 0,
		  NOTE_C4, NOTE_D4, NOTE_G4, NOTE_FS4, NOTE_DS4,
		  NOTE_C4, NOTE_D4
};

const uint32_t marioDuration[] = {
		  12, 12, 12, 12,
		  12, 12, 12, 12,
		  12, 12, 12, 12,
		  12, 12, 12, 12,

		  12, 12, 12, 12,
		  12, 12, 12, 12,
		  12, 12, 12, 12,
		  12, 12, 12, 12,

		  9, 9, 9,
		  12, 12, 12, 12,
		  12, 12, 12, 12,
		  12, 12, 12, 12,

		  12, 12, 12, 12,
		  12, 12, 12, 12,
		  12, 12, 12, 12,
		  12, 12, 12, 12,

		  9, 9, 9,
		  12, 12, 12, 12,
		  12, 12, 12, 12,
		  12, 12, 12, 12,
};

const uint32_t secondDuration[] = {
  8,4,8,4,
  4,4,4,12,
  4,4,4,4,4,4,
  4,16,4,

  8,4,8,4,
  4,2,1,1,2,1,1,12,
  4,4,4,4,4,4,
  4,16,4,

  4,4,4,4,4,4,
  4,4,4,12,
  4,4,4,4,4,4,
  4,4,4,12,

  4,4,4,4,4,4,
  2,1,1,2,1,1,4,8,4,
  2,6,4,2,6,4,
  2,1,1,16,4,

  4,8,4,4,4,
  4,16,4,
  4,8,4,4,4,
  4,20,
};

/******************************************************************************/
/*                            EXPORTED FUNCTIONS                              */
/******************************************************************************/
void play_note(uint32_t freq, uint32_t duration) {
    BuzzerControl_SetFreqMelody(freq);
    delay_ms(duration);
    BuzzerControl_SetFreqMelody(0); // Stop the note after the duration
}

void play_song() {
		uint32_t la1 = 440; // A4
	    uint32_t sol1 = 392; // G4
	    uint32_t Do2 = 523; // C5
	    uint32_t re2 = 587; // D5
	    uint32_t mi2 = 659; // E5
	    uint32_t si1 = 493; // B4
	    uint32_t re1 = 293; // D4
	    uint32_t fa2 = 698; //F5
	    uint32_t sol2 = 784; //G5
	    uint32_t la2 = 880; //A5

	    //Em la ai
	    play_note(la1, NOTE_LEN); play_note(sol1, NOTE_LEN); play_note(la1, NOTE_LEN); play_note(Do2, NOTE_LEN); play_note(re2, NOTE_LEN); play_note(mi2, NOTE_LEN); play_note(mi2, NOTE_LEN); play_note(re2, NOTE_LEN); play_note(Do2, NOTE_LEN); play_note(si1, NOTE_LEN); play_note(si1, NOTE_LEN); play_note(Do2, NOTE_LEN); play_note(Do2, NOTE_LEN*3);
	    play_note(la1, NOTE_LEN); play_note(sol1, NOTE_LEN); play_note(la1, NOTE_LEN); play_note(Do2, NOTE_LEN); play_note(re2, NOTE_LEN); play_note(mi2, NOTE_LEN); play_note(mi2, NOTE_LEN); play_note(re2, NOTE_LEN); play_note(Do2, NOTE_LEN); play_note(si1, NOTE_LEN); play_note(si1, NOTE_LEN); play_note(Do2, NOTE_LEN); play_note(Do2, NOTE_LEN*3);
	    play_note(Do2, NOTE_LEN); play_note(si1, NOTE_LEN); play_note(la1, NOTE_LEN*2); play_note(re2, NOTE_LEN + NOTE_LEN+NOTE_LEN/2); play_note(si1, NOTE_LEN/2); play_note(si1, NOTE_LEN); play_note(Do2, NOTE_LEN); play_note(re2, NOTE_LEN); play_note(sol1, NOTE_LEN*2);
	    play_note(Do2, NOTE_LEN); play_note(si1, NOTE_LEN); play_note(la1, NOTE_LEN); play_note(mi2, NOTE_LEN); play_note(mi2, NOTE_LEN*2); play_note(la1, NOTE_LEN); play_note(re1, NOTE_LEN); play_note(re1, NOTE_LEN*2); play_note(Do2, NOTE_LEN); play_note(Do2, NOTE_LEN*2);

	    //Bao ngay qua
	    play_note(la1, NOTE_LEN); play_note(sol1, NOTE_LEN); play_note(la1, NOTE_LEN); play_note(Do2, NOTE_LEN); play_note(re2, NOTE_LEN); play_note(mi2, NOTE_LEN); play_note(mi2, NOTE_LEN); play_note(re2, NOTE_LEN); play_note(Do2, NOTE_LEN); play_note(si1, NOTE_LEN); play_note(si1, NOTE_LEN); play_note(Do2, NOTE_LEN); play_note(Do2, NOTE_LEN*3);
	    play_note(la1, NOTE_LEN); play_note(sol1, NOTE_LEN); play_note(la1, NOTE_LEN); play_note(Do2, NOTE_LEN); play_note(re2, NOTE_LEN); play_note(mi2, NOTE_LEN); play_note(mi2, NOTE_LEN); play_note(re2, NOTE_LEN); play_note(Do2, NOTE_LEN); play_note(si1, NOTE_LEN); play_note(si1, NOTE_LEN); play_note(Do2, NOTE_LEN); play_note(Do2, NOTE_LEN*3);
	    play_note(Do2, NOTE_LEN); play_note(si1, NOTE_LEN); play_note(la1, NOTE_LEN*2); play_note(re2, NOTE_LEN + NOTE_LEN+NOTE_LEN/2); play_note(si1, NOTE_LEN/2); play_note(si1, NOTE_LEN); play_note(Do2, NOTE_LEN); play_note(re2, NOTE_LEN); play_note(sol1, NOTE_LEN*2);
	    play_note(Do2, NOTE_LEN); play_note(si1, NOTE_LEN); play_note(la1, NOTE_LEN); play_note(mi2, NOTE_LEN); play_note(mi2, NOTE_LEN*2); play_note(la1, NOTE_LEN); play_note(re2, NOTE_LEN); play_note(re2, NOTE_LEN*2); play_note(Do2, NOTE_LEN); play_note(Do2, NOTE_LEN*2);

	    //Cam tay anh
	    play_note(mi2, NOTE_LEN*2); play_note(fa2, NOTE_LEN); play_note(sol2, NOTE_LEN*2); play_note(mi2, NOTE_LEN); play_note(fa2, NOTE_LEN); play_note(sol2, NOTE_LEN*2);
	    play_note(mi2, NOTE_LEN); play_note(fa2, NOTE_LEN); play_note(sol2, NOTE_LEN*2); play_note(sol2, NOTE_LEN); play_note(mi2, NOTE_LEN); play_note(la2, NOTE_LEN); play_note(sol2, NOTE_LEN*2);
	    play_note(fa2, NOTE_LEN); play_note(sol2, NOTE_LEN); play_note(mi2, NOTE_LEN); play_note(re2, NOTE_LEN); play_note(Do2, NOTE_LEN*2); play_note(re2, NOTE_LEN);
	    play_note(sol1, NOTE_LEN); play_note(mi2, NOTE_LEN); play_note(re2, NOTE_LEN); play_note(Do2, NOTE_LEN); play_note(re2, NOTE_LEN); play_note(Do2, NOTE_LEN); play_note(re2, NOTE_LEN); play_note(sol2, NOTE_LEN*2); play_note(mi2, NOTE_LEN);


	    //Cam tay anh
	    play_note(mi2, NOTE_LEN*2); play_note(fa2, NOTE_LEN); play_note(sol2, NOTE_LEN*2); play_note(mi2, NOTE_LEN); play_note(fa2, NOTE_LEN); play_note(sol2, NOTE_LEN*2);
	    play_note(mi2, NOTE_LEN); play_note(fa2, NOTE_LEN); play_note(sol2, NOTE_LEN*2); play_note(sol2, NOTE_LEN); play_note(mi2, NOTE_LEN*2); play_note(la2, NOTE_LEN); play_note(sol2, NOTE_LEN*2);
	    play_note(fa2, NOTE_LEN); play_note(sol2, NOTE_LEN); play_note(mi2, NOTE_LEN); play_note(re2, NOTE_LEN); play_note(Do2, NOTE_LEN); play_note(re2, NOTE_LEN);
	    play_note(sol1, NOTE_LEN); play_note(mi2, NOTE_LEN); play_note(re2, NOTE_LEN); play_note(Do2, NOTE_LEN); play_note(re2, NOTE_LEN); play_note(mi2, NOTE_LEN); play_note(mi2, NOTE_LEN); play_note(sol2, NOTE_LEN); play_note(mi2, NOTE_LEN); play_note(re2, NOTE_LEN); play_note(Do2, NOTE_LEN*2);



	    //Mua xuan den binh yen
	    play_note(Do2, NOTE_LEN/2); play_note(re2, NOTE_LEN); play_note(mi2, NOTE_LEN); play_note(la1, NOTE_LEN*2); play_note(re2, NOTE_LEN*1.5);
	    play_note(re2, NOTE_LEN/2); play_note(re2, NOTE_LEN); play_note(mi2, NOTE_LEN); play_note(fa2, NOTE_LEN); play_note(mi2, NOTE_LEN*2);
	    play_note(Do2, NOTE_LEN); play_note(re2, NOTE_LEN); play_note(mi2, NOTE_LEN); play_note(la1, NOTE_LEN*2); play_note(re2, NOTE_LEN*1.5);
	    play_note(si1, NOTE_LEN/2); play_note(si1, NOTE_LEN); play_note(Do2, NOTE_LEN); play_note(Do2, NOTE_LEN);
	    play_note(Do2, NOTE_LEN/2); play_note(re2, NOTE_LEN); play_note(mi2, NOTE_LEN); play_note(la1, NOTE_LEN*2); play_note(re2, NOTE_LEN*1.5);
	    play_note(re2, NOTE_LEN/2); play_note(re2, NOTE_LEN); play_note(mi2, NOTE_LEN); play_note(fa2, NOTE_LEN); play_note(mi2, NOTE_LEN*2);
	    play_note(Do2, NOTE_LEN); play_note(re2, NOTE_LEN); play_note(mi2, NOTE_LEN); play_note(la1, NOTE_LEN*2); play_note(re2, NOTE_LEN*1.5);
	    play_note(si1, NOTE_LEN/2); play_note(si1, NOTE_LEN); play_note(Do2, NOTE_LEN); play_note(Do2, NOTE_LEN);
}

const uint32_t* melody[] ={secondMelody};
const uint32_t* noteDurations[] = {secondDuration};
const uint16_t melodySlowfactor[] ={30};

const uint32_t melodySizes[] ={sizeof(secondDuration)/sizeof(uint32_t)};
