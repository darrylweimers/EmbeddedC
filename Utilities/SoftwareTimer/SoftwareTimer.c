//***********************************************
//	SoftwareTimer.c
//***********************************************

// Note:		1. Requires a micro-controller timer set to 1 nanosecond, microseconds, or milliseconds

#include "SoftwareTimer.h"

/* ********************************************************************
 * PRIVATE FUNCTION DECLARATIONS
 * *********************************************************************/

static void CounterIncrement(SoftwareTimer* softwareTimer, U8 Module);
static void CounterEnable(SoftwareTimer* softwareTimer, U8 Module);
static void CounterDisable(SoftwareTimer* softwareTimer, U8 Module);
static bool ModuleExists(SoftwareTimer* softwareTimer, U8 Module);
static void SoftwareTimer_DefautSetting(SoftwareTimer* softwareTimer);

/* ********************************************************************
 * PRIVATE FUNCTION DEFINITIONS
 * *********************************************************************/

// Brief:		Set all software timer to default setting
// Param1:		Software timer properties
// Return:		Void
static void SoftwareTimer_DefautSetting(SoftwareTimer* softwareTimer)
{
	U8 i;

	for(i = 0; i < MAX_NUMBER_OF_MODULES; i++)
	{
		softwareTimer->enabled[i] = FALSE;
		softwareTimer->count[i] = 0;
		softwareTimer->max[i] = 0;
		softwareTimer->type[i] = SoftwareTimer_Type_Null;
	}

	softwareTimer->totalModuleEnabled = 0;
}

// Brief:		Brief
// Param1:		Software timer properties
// Param2:		Software counter id
// Return:		Void
static bool ModuleExists(SoftwareTimer* softwareTimer, U8 Module)
{
	if (Module < softwareTimer->numberOfModule)
		return TRUE;

	return FALSE;
}

// Brief:		Increment all enabled software counter
// Param1:		Software timer properties
// Param2:		Software counter id
// Return:		Void
static void CounterIncrement(SoftwareTimer* softwareTimer, U8 Module)
{
	if (softwareTimer->enabled[Module] == TRUE)
		softwareTimer->count[Module] += 1;		// increment by 1 us or 1 ms
}

// Brief:		Eanble software counter and micro-controller timer module if it is not enabled
// Param1:		Software timer properties
// Param2:		Software counter id
// Return:		Void
static void CounterEnable(SoftwareTimer* softwareTimer, U8 Module)
{
	if (ModuleExists(softwareTimer, Module) && softwareTimer->enabled[Module] == FALSE)
	{
		softwareTimer->enabled[Module] = TRUE;
		softwareTimer->totalModuleEnabled++;

		if (softwareTimer->totalModuleEnabled == 1)
			(*softwareTimer->driverEnable)(ENABLE);		// Enables/Disables the 1 us or 1 ms timer counter
	}
}

// Brief:		Disable software counter and micro-controller timer module if all software counter is disabled
// Param1:		Software timer properties
// Param2:		Software counter id
// Return:		Void
static void CounterDisable(SoftwareTimer* softwareTimer, U8 Module)
{
	if (ModuleExists(softwareTimer, Module) && softwareTimer->enabled[Module] == TRUE)
	{
		softwareTimer->totalModuleEnabled--;
		softwareTimer->enabled[Module] = FALSE;
		if (softwareTimer->totalModuleEnabled == 0)
			(*softwareTimer->driverEnable)(DISABLE);
	}
}

/* ********************************************************************
 * PUBLIC FUNCTIONS DEFINITIONS
 * *********************************************************************/

// Brief:		Determine if a software counter reaches its predefined max duration
// Param1:		Software timer properties
// Param2:		Software counter id
// Return:		TRUE (software counter reaches its predefined max)/ FALSE
bool SoftwareTimer_IsTriggered(SoftwareTimer* softwareTimer, U8 Module)
{
	if (ModuleExists(softwareTimer, Module) && softwareTimer->driverEnabled && softwareTimer->enabled[Module] == TRUE)
	{
			if(softwareTimer->count[Module] >= softwareTimer->max[Module])
			{
				// For counter to run periodically variable "counterTriggered" must be set to FALSE
				if (softwareTimer->type[Module] == SoftwareTimer_Type_Periodic)
				{

				}else if (softwareTimer->type[Module] == SoftwareTimer_Type_Timeout)
				{
					// Disable the system
					CounterDisable(softwareTimer, Module);
				}

				softwareTimer->count[Module] = 0;
				return TRUE;
			}
	}

	return FALSE;
}

// Brief:		Configure using platforms setting
// Param1:		Software timer properties
// Param2:		Total number of software timer defined by user
// Param3:		Function to configure micro-controller timer module
// Param4:		Function to enable/disable micro-controller timer module
// Return:		Void
void SoftwareTimer_DriverInit(SoftwareTimer* softwareTimer, U8 TotalNumberOfModule, Func_Ptr_Void DriverInit,  Func_Ptr_Enable DriverEnable)
{
	// Allow user to call this function once with execution
	if (softwareTimer->driverEnabled == FALSE)
	{

		SoftwareTimer_DefautSetting(softwareTimer);

		if  (TotalNumberOfModule > MAX_NUMBER_OF_MODULES)
			softwareTimer->numberOfModule = MAX_NUMBER_OF_MODULES;
		else
			softwareTimer->numberOfModule = TotalNumberOfModule;

		softwareTimer->driverEnable = DriverEnable;

		// Timer init
		(*DriverInit)();

		// Timer module state
		(*DriverEnable)(DISABLE);

		// Once TC initialization is successfully prevent user from re-initializing it
		softwareTimer->driverEnabled = TRUE;

	}
}


// Brief:		Enable a software counter
// Param1:		Software timer properties
// Param2:		Software counter id
// Return:		Void
void SoftwareTimer_Start(SoftwareTimer* softwareTimer, U8 Module)
{
	if (ModuleExists(softwareTimer, Module) && softwareTimer->driverEnabled)
	{
		if ((softwareTimer->type[Module] == SoftwareTimer_Type_Timeout) || (softwareTimer->type[Module] == SoftwareTimer_Type_Periodic))
		{
			softwareTimer->count[Module] = 0;
			CounterEnable(softwareTimer, Module);
		}
	}
}

// Brief:		Disable a software counter
// Param1:		Software timer properties
// Param2:		Software counter id
// Return:		Void
void SoftwareTimer_Stop(SoftwareTimer* softwareTimer, U8 Module)
{
	if (ModuleExists(softwareTimer, Module) && softwareTimer->driverEnabled && softwareTimer->totalModuleEnabled > 0)
		CounterDisable(softwareTimer, Module);
}

// Brief:		Configure a software timer
// Param1:		Software timer properties
// Param2:		Software counter id
// Param3:		Max count value
// Param4:		Periodic/Timeout
// Return:		Void
void SoftwareTimer_Init(SoftwareTimer* softwareTimer, U8 Module, U32 Max, SoftwareTimer_Type Type)
{
	// counter must be initially disabled, if it is has been enabled, second initialization will be omitted
	if (ModuleExists(softwareTimer, Module) && softwareTimer->enabled[Module] == FALSE)
	{
			softwareTimer->max[Module] = Max;
			softwareTimer->type[Module] = Type;
			softwareTimer->count[Module] = 0;
	}
}

// Brief:		Update software timers
// Param1:		Void
// Return:		Void
// Note:		1. Must be call by micro-controller timer ISR
void SoftwareTimer_InterruptHanlder(SoftwareTimer* softwareTimer)
{
	U8 i;
	for(i = 0; i < softwareTimer->numberOfModule; i++)
		CounterIncrement(softwareTimer, i);
}
