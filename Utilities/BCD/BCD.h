//*************************
// BCD.h
//*************************

#ifndef _BCD_H_ 
#define _BCD_H_ 

#include "GlobalDef.h"

/* *************************************************************************
 * PUBLIC FUNCTIONS DECLARATIONS
 * *************************************************************************
 */

U32 BCD_GetTime(void);
U8 BCD_GetTime_Second(void);
U8 BCD_GetTime_Minute(void);
U8 BCD_GetTime_Hour(void);
void BCD_ResetTime(void);
void BCD_UpdateTime(void);

#endif 
