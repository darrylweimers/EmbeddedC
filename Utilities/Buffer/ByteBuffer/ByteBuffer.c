//***********************************************
// 	ByteBuffer.c
//***********************************************

#include "ByteBuffer.h"

/* ********************************************************************
 * DEFINES
 * *********************************************************************/

#define TYPE_FRAME_SIZE_LENGTH	4			// Number of bytes use to store frame size
#define TYPE_CHECK_SUM_LENGTH 	1			// Number of byte use to store check sum value

/* ********************************************************************
 * PRIVATE FUNCTION DECLARATIONS
 * *********************************************************************/

static U8 ReadByte(volatile FIFO_ByteBuffer* Fifo, volatile U8* Byte);
static U8 StoreByte(volatile FIFO_ByteBuffer * Fifo, U8 Byte);
static U8 Push(volatile FIFO_ByteBuffer * Fifo, U8 Byte);
static U8 Pop(volatile FIFO_ByteBuffer * Fifo, volatile U8* Byte);
static void Buffer_SetToDefault(volatile FIFO_ByteBuffer * Fifo);

/* ********************************************************************
 * PRIVATE FUNCTION DEFINITIONS
 * *********************************************************************/

// Read a byte from buffer
static U8 ReadByte(volatile FIFO_ByteBuffer* Fifo, volatile U8* Byte)
{
	// Read byte from buffer
	*Byte = Fifo->Fifo[Fifo->ReadPtr++];

	// Decrement the total number of elements
	Fifo->TotalElement--;

	// Wrap around to the beginning when the end is reached
	if(Fifo->ReadPtr >= BUFFER_SIZE)
		Fifo->ReadPtr = 0;

	return *Byte;
}

// Store a byte into buffer
static U8 StoreByte(volatile FIFO_ByteBuffer * Fifo, U8 Byte)
{
	// Store frame size MSB
	Fifo->Fifo[Fifo->WritePtr++] = Byte;

	// Increment total byte count
	Fifo->TotalElement++;

	// Wrap around to beginning of FIFO_Byte buffer when buffer last element is surpassed
	if(Fifo->WritePtr >= BUFFER_SIZE)
	Fifo->WritePtr = 0;

	return Byte;
}

// Store a frame to buffer
static U8 Push(volatile FIFO_ByteBuffer * Fifo, U8 Byte)
{
	// If we have more storage space
	if (Fifo->TotalElement <= BUFFER_SIZE)
	{
		StoreByte(Fifo,Byte);

		// Task Completed Successfully
		return BYTE_BUFFER_FIFO_OK;
	}

	return BYTE_BUFFER_FIFO_FULL;
}

// Get a frame from buffer
static U8 Pop(volatile FIFO_ByteBuffer * Fifo, volatile U8* Byte)
{
	// If we have elements
	if (Fifo->TotalElement > 0)
	{
		ReadByte(Fifo, Byte);

		// Task Completed Successfully
		return BYTE_BUFFER_FIFO_OK;
	}
	return BYTE_BUFFER_FIFO_EMPTY;
}

// Set fifo variables to their default values
static void Buffer_SetToDefault(volatile FIFO_ByteBuffer * Fifo)
{
	Fifo->ReadPtr = 0;
	Fifo->WritePtr = 0;
	Fifo->TotalElement = 0;
}

/* ********************************************************************
 * PUBLIC FUNCTION DEFINITIONS
 * *********************************************************************/

// Sets FIFO tx and rx to their default values
void ByteBuffer_Init(volatile FIFO_ByteBuffer * Fifo)
{
	Buffer_SetToDefault(Fifo);
}

U8 ByteBuffer_Push(volatile FIFO_ByteBuffer * Fifo, U8 Byte)
{
	return Push(Fifo, Byte);
}

U8 ByteBuffer_Pop(volatile FIFO_ByteBuffer * Fifo, volatile U8* Byte)
{
	return Pop(Fifo, Byte);
}
