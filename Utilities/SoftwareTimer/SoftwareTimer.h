//***********************************************
//	SoftwareTimer.h
//***********************************************

#ifndef _SOFTWARETIMER_H_
#define _SOFTWARETIMER_H_

#include "GlobalDef.h"
#include "ADSTypes.h"

/* ********************************************************************
 * DEFINE
 * *********************************************************************/

#define TOTAL_NUMBER_OF_SOFWARE_TIMER					8

/* ********************************************************************
 * TYPE
 * *********************************************************************/

// Each module can be set to mode: periodic, timeout
typedef enum
{
	SoftwareTimer_Type_Null,
	SoftwareTimer_Type_Periodic,
	SoftwareTimer_Type_Timeout
}SoftwareTimer_Type;

typedef struct
{
	volatile bool enabled[TOTAL_NUMBER_OF_SOFWARE_TIMER];
	volatile U32 count[TOTAL_NUMBER_OF_SOFWARE_TIMER];
	U32 max[TOTAL_NUMBER_OF_SOFWARE_TIMER];
	SoftwareTimer_Type type[TOTAL_NUMBER_OF_SOFWARE_TIMER];
	U8 totalModuleEnabled;
	bool driverEnabled;
	Func_Ptr_Enable driverEnable;
	U8 numberOfModule;
}SoftwareTimer;

/* ********************************************************************
 * PUBLIC FUNCTIONS DECLARATIONS
 * *********************************************************************/

bool SoftwareTimer_IsTriggered(SoftwareTimer* softwareTimer, U8 Module);
void SoftwareTimer_Start(SoftwareTimer* softwareTimer, U8 Module);
void SoftwareTimer_Stop(SoftwareTimer* softwareTimer, U8 Module);
void SoftwareTimer_Init(SoftwareTimer* softwareTimer, U8 Module, U32 Max, SoftwareTimer_Type Type);
void SoftwareTimer_InterruptHanlder(SoftwareTimer* softwareTimer);
void SoftwareTimer_DriverInit(SoftwareTimer* softwareTimer,U8 TotalNumberOfModule, Func_Ptr_Void DriverInit,  Func_Ptr_Enable DriverEnable);

#endif /* _SOFTWARETIMER_H_ */
