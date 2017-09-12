//***********************************************
//	BitTwiddling.h
//***********************************************

#ifndef BIT_TWIDDLING_H_
#define BIT_TWIDDLING_H_

#include "BitTwiddler.h"

#define BIT_IS_SET(sfr,bit)									(_SFR_BYTE(sfr) & BIT_TWIDDLER_BIT_VALUE(bit))
#define BIT_IS_CLEAR(sfr,bit)								(!(_SFR_BYTE(sfr) & BIT_TWIDDLER_BIT_VALUE(bit)))
#define LOOP_UNTIL_BIT_IS_SET(sfr,bit)						do { } while (BIT_IS_CLEAR(sfr,bit))
#define LOOP_UNTIL_BIT_IS_CLEAR(sfr,bit)					do {} while (BIT_IS_SET(sfr,bit))

#endif /* BIT_TWIDDLING_H_ */
