//*****************************************
// Array.c
//*****************************************

#include "Array.h"

/* *************************************************************************
 * PUBLIC FUNCTIONS DEFINITIONS
 * *************************************************************************
 */
 
// Fill up buffer with value counting from starting value to 255 (and wrap around to 0 when reach max value 255) 
void Array_FillWithRandomValues(U8 startingValue,U8 buffer[],U32 bufferSize)
{
	U8 counter = startingValue; 
	U32 index; 
	
	for(index = 0; index < bufferSize ; index++)
		buffer[index] = counter++; 
}

// Fill up buffer with value
void Array_FillWithValue(U8 value,U8 buffer[],U32 bufferSize)
{
	U32 index; 
	
	for(index = 0; index < bufferSize ; index++)
		buffer[index] = value; 
}

// Assuming that buffering 1 and 2 are of the same size, this function verify if all elements are identical 
// Return value: Identical (1) / Different (0)
U8 Array_Compare(U8* pBuffer1, U8* pBuffer2, U32 bufferSize)
{
  while (bufferSize--)
  {
    if (*pBuffer1 != *pBuffer2)
    {
      return 0;
    }

    pBuffer1++;
    pBuffer2++;
  }

  return 1;
}
