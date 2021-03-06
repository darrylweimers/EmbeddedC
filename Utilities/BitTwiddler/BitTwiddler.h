
//*****************************************
// BitTwiddler.h
//*****************************************

#ifndef _BITTWIDDLER_H_
#define _BITTWIDDLER_H_

/* *************************************************************************
 * DEFINES
 * *************************************************************************
 */

#define BIT_TWIDDLER_BIT_VALUE(BIT_POS)			(1 << BIT_POS)
#define BIT_TWIDDLER_SET_BIT(BYTE, BIT_POS)   	(BYTE | BIT_TWIDDLER_BIT_VALUE(BIT_POS))
#define BIT_TWIDDLER_CLEAR_BIT(BYTE, BIT_POS) 	(BYTE & (~BIT_TWIDDLER_BIT_VALUE(BIT_POS)))
#define BIT_TWIDDLER_TOGGLE_BIT(BYTE, BIT_POS) 	(BYTE ^ BIT_TWIDDLER_BIT_VALUE(BIT_POS))
#define BIT_TWIDDLER_GET_BIT(BYTE, BIT_POS)   	((BYTE & BIT_TWIDDLER_BIT_VALUE(BIT_POS)) >> BIT_POS)

#endif /* _BITTWIDDLER_H_ */
