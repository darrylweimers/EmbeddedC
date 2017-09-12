//*****************************************
// Array.h
//*****************************************

#ifndef _ARRAY_H_
#define	_ARRAY_H_

#include "GlobalDef.h"

/* *************************************************************************
 * PUBLIC FUNCTIONS DECLARATIONS
 * *************************************************************************
 */
 
void Array_FillWithRandomValues(U8 startingValue,U8 buffer[],U32 bufferSize);
void Array_FillWithValue(U8 value,U8 buffer[],U32 bufferSize);
U8 Array_Compare(U8* pBuffer1, U8* pBuffer2, U32 bufferSize);

#endif /* _ARRAY_H_ */
