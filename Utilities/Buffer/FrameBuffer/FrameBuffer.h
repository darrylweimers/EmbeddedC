//***********************************************
// 	FrameBuffer.h
//***********************************************

#ifndef _FRAMEBUFFER_H_
#define _FRAMEBUFFER_H_

#include "GlobalDef.h"

/* ********************************************************************
 * DEFINES
 * *********************************************************************/

#define FRAME_BUFFER_FIFO_FULL	FAIL
#define FRAME_BUFFER_FIFO_EMPTY	FAIL
#define FRAME_BUFFER_FIFO_OK	PASS

#define FIFO_BUFFER_SIZE 2048		// MARK: determine value

/* ********************************************************************
 * TYPES
 * *********************************************************************/

typedef volatile struct
{
	volatile U32 TotalElement;				// Total of elements in fifo array
	volatile U16 FrameCount;				// Total number of frame in fifo array
	volatile U32 ReadPtr;					// Read location
	volatile U32 WritePtr;					// Write location
	volatile U8  Fifo[FIFO_BUFFER_SIZE];	// Queue
}FIFO_FrameBuffer;

/* ********************************************************************
 * PUBLIC FUNCTION DECLARATIONS
 * *********************************************************************/

void FrameBuffer_Init(volatile FIFO_FrameBuffer * FrameFifo);
U8 FrameBuffer_Push(volatile FIFO_FrameBuffer * FrameFifo, U8 Frame[], U32 SizeOfFrame);
U8 FrameBuffer_Pop(volatile FIFO_FrameBuffer * FrameFifo, volatile U8 Frame[], volatile U32* SizeOfFrame);

#endif /* FRAMEBUFFER_H_ */
