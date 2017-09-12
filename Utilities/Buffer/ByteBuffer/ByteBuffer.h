//***********************************************
// 	ByteBuffer.h
//***********************************************

#ifndef BYTEBUFFER_H_
#define BYTEBUFFER_H_

#include "compiler.h"

/* ********************************************************************
 * DEFINES
 * *********************************************************************/

#define BYTE_BUFFER_FIFO_FULL		FAIL
#define BYTE_BUFFER_FIFO_EMPTY	FAIL
#define BYTE_BUFFER_FIFO_OK			PASS

#define BUFFER_SIZE 	1000		  // MARK: determine value

/* ********************************************************************
 * TYPES
 * *********************************************************************/

typedef volatile struct
{
	volatile U32 TotalElement;				// Total of elements in fifo array
	volatile U32 ReadPtr;					    // Read location
	volatile U32 WritePtr;					  // Write location
	volatile U8  Fifo[BUFFER_SIZE];		// Queue
}FIFO_ByteBuffer;

/* ********************************************************************
 * PUBLIC FUNCTION DECLARATIONS
 * *********************************************************************/

void ByteBuffer_Init(volatile FIFO_ByteBuffer * Fifo);
U8 ByteBuffer_Push(volatile FIFO_ByteBuffer * Fifo, U8 Byte);
U8 ByteBuffer_Pop(volatile FIFO_ByteBuffer * Fifo, volatile U8* Byte);

#endif /* BYTEBUFFER_H_ */
