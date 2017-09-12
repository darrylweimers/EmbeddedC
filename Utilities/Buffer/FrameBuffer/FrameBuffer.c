//***********************************************
// 	FrameBuffer.c
//***********************************************

#include "FrameBuffer.h"

/* ********************************************************************
 * DEFINES
 * *********************************************************************/

#define TYPE_FRAME_SIZE_LENGTH	4			// Number of bytes use to store frame size
#define TYPE_CHECK_SUM_LENGTH 	1			// Number of byte use to store check sum value

/* ********************************************************************
 * PRIVATE FUNCTION DECLARATIONS
 * *********************************************************************/

static U8 ReadByte(volatile FIFO_FrameBuffer* Fifo, volatile U8* Byte);
static U8 StoreByte(volatile FIFO_FrameBuffer * Fifo, U8 Byte);
static U8 Push(volatile FIFO_FrameBuffer * Fifo, U8 Frame[], U32 SizeOfFrame);
static U8 Pop(volatile FIFO_FrameBuffer * Fifo, volatile U8 FrameDst[], volatile U32* SizeOfFrameDst);
static void FrameBuffer_SetToDefault(volatile FIFO_FrameBuffer * Fifo);

/* ********************************************************************
 * PRIVATE FUNCTION DEFINITIONS
 * *********************************************************************/

// Read a byte from buffer
static U8 ReadByte(volatile FIFO_FrameBuffer* Fifo, volatile U8* Byte)
{
	// Read byte from buffer
	*Byte = Fifo->Fifo[Fifo->ReadPtr++];

	// Decrement the total number of elements
	Fifo->TotalElement--;

	// Wrap around to the beginning when the end is reached
	if(Fifo->ReadPtr >= FIFO_BUFFER_SIZE)
		Fifo->ReadPtr = 0;

	return *Byte;
}

// Store a byte into buffer
static U8 StoreByte(volatile FIFO_FrameBuffer * Fifo, U8 Byte)
{
	// Store frame size MSB
	Fifo->Fifo[Fifo->WritePtr++] = Byte;

	// Increment total byte count
	Fifo->TotalElement++;

	// Wrap around to beginning of FIFO buffer when buffer last element is surpassed
	if(Fifo->WritePtr >= FIFO_BUFFER_SIZE)
	Fifo->WritePtr = 0;

	return Byte;
}

// Store a frame to buffer
static U8 Push(volatile FIFO_FrameBuffer * Fifo, U8 Frame[], U32 SizeOfFrame)
{
	// Set Index to zero
	U32 i = 0;
	U8 checkSum = 0;

	// If we have more storage space
	if (SizeOfFrame + TYPE_FRAME_SIZE_LENGTH + TYPE_CHECK_SUM_LENGTH <= FIFO_BUFFER_SIZE - Fifo->TotalElement)
	{
		// Store frame size
		checkSum += StoreByte(Fifo, (U8)(SizeOfFrame >> 24));
		checkSum += StoreByte(Fifo, (U8)(SizeOfFrame >> 16));
		checkSum += StoreByte(Fifo, (U8)(SizeOfFrame >> 8));
		checkSum += StoreByte(Fifo, (U8)SizeOfFrame);

		// Store frame
		for (i = 0; i < SizeOfFrame; i++)
			checkSum += StoreByte(Fifo, Frame[i]);

		// Store check sum
		StoreByte(Fifo, (U8) checkSum);

		// Increment frame stored
		Fifo->FrameCount++;

		// Task Completed Successfully
		return FRAME_BUFFER_FIFO_OK;
	}

	return FRAME_BUFFER_FIFO_FULL;
}

// Get a frame from buffer
static U8 Pop(volatile FIFO_FrameBuffer * Fifo, volatile U8 FrameDst[], volatile U32* SizeOfFrameDst)
{
	// Set Index to zero
	U16 i = 0;
	U8 checkSum = 0;
	U8 tempByte = 0;

	// If we have elements
	if (Fifo->TotalElement > 0 && Fifo->FrameCount > 0)
	{
		// Clear original data
		*SizeOfFrameDst = 0;

		// Retrieve frame size
		checkSum += ReadByte(Fifo, &tempByte);
		*SizeOfFrameDst += ((U32)tempByte << 24);

		checkSum += ReadByte(Fifo, &tempByte);
		*SizeOfFrameDst += ((U32)tempByte << 16);

		checkSum += ReadByte(Fifo, &tempByte);
		*SizeOfFrameDst += ((U32)tempByte << 8);

		checkSum += ReadByte(Fifo, &tempByte);
		*SizeOfFrameDst += ((U32)tempByte);

		// Retrieve frame
		for (i = 0; i < *SizeOfFrameDst; i++)
			checkSum += ReadByte(Fifo, &FrameDst[i]);

		// Decrement frame stored
		Fifo->FrameCount--;

		// Verify if check sum is accurate
		if (checkSum != ReadByte(Fifo, &tempByte))
			return 0; 	// TODO: Check Sum error

		// Task Completed Successfully
		return FRAME_BUFFER_FIFO_OK;
	}

	return FRAME_BUFFER_FIFO_EMPTY;
}

// Set fifo variables to their default values
static void FrameBuffer_SetToDefault(volatile FIFO_FrameBuffer * Fifo)
{
	Fifo->FrameCount = 0;
	Fifo->ReadPtr = 0;
	Fifo->WritePtr = 0;
	Fifo->TotalElement = 0;
}

/* ********************************************************************
 * PUBLIC FUNCTION DEFINITIONS
 * *********************************************************************/

// Sets FIFO tx and rx to their default values
void FrameBuffer_Init(volatile FIFO_FrameBuffer * FrameFifo)
{
	FrameBuffer_SetToDefault(FrameFifo);
}

U8 FrameBuffer_Push(volatile FIFO_FrameBuffer * FrameFifo, U8 Frame[], U32 SizeOfFrame)
{
	return Push(FrameFifo, Frame, SizeOfFrame);
}

U8 FrameBuffer_Pop(volatile FIFO_FrameBuffer * FrameFifo, volatile U8 Frame[], volatile U32* SizeOfFrame)
{
	return Pop(FrameFifo, Frame, SizeOfFrame);
}
