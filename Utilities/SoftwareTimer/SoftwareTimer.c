//***********************************************
//	SoftwareTimer.c
//***********************************************

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



static bool ModuleExists(SoftwareTimer* softwareTimer, U8 Module)
{
	if (Module < softwareTimer->numberOfModule)
		return TRUE;

	return FALSE;
}

/*
 *
 * @brief
 * Add Timer counter Module period to software counter count value
 *
 * @param
 * Provide Software counter associated number
 *
 * @return
 * void
 */
static void CounterIncrement(SoftwareTimer* softwareTimer, U8 Module)
{
	if (softwareTimer->enabled[Module] == TRUE)
		softwareTimer->count[Module] += 1;		// increment by 1 us or 1 ms
}

/*
 *
 * @brief
 * Enable software counter if it has been disabled. And, enable Timer counter module if at least one software counter has been enabled.
 *
 * @param counterNumber
 * Provide Software counter associated number
 *
 * @return
 * void
 */
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

/*
 * @brief
 * Disable software counter if it has been enabled. And, disable Timer counter module if all software counter has been disabled.
 *
 * @param
 * Provide Software counter associated number
 *
 * @return
 * void
 */
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

/*
 * @brief
 * Determine if a software counter reaches its predefined max duration
 *
 * @param
 * Provide the software counter to commit the sequence of tasks
 *
 * @return
 * TRUE (software counter reaches its predefined max)/ FALSE
 */
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

/*!
 *
 * @brief
 * Enable a software counter associated with the input number. And, set the count value to zero (default value)
 *
 * @param
 * Provide the software counter to commit the sequence of tasks
 *
 * @return
 * void
 */
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

/*!
 *
 * @brief
 * Disable a software counter associated with the input number
 *
 * @param
 * Provide the software counter to commit the sequence of tasks
 *
 * @return
 * void
 */
void SoftwareTimer_Stop(SoftwareTimer* softwareTimer, U8 Module)
{
	if (ModuleExists(softwareTimer, Module) && softwareTimer->driverEnabled && softwareTimer->totalModuleEnabled > 0)
		CounterDisable(softwareTimer, Module);
}

/*
 * @brief
 * Initialize a software counter
 *
 * @param
 * Provide the software counter to commit the sequence of tasks
 *
 * @param
 * Set period or duration is milliseconds
 *
 * @param
 * Set Timer type which can be select from: type_Periodic,type_Timeout
 *
 * @return
 * void
 */
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

// Call function under TIMx counter interrupt
void SoftwareTimer_InterruptHanlder(SoftwareTimer* softwareTimer)
{
	U8 i;
	for(i = 0; i < softwareTimer->numberOfModule; i++)
		CounterIncrement(softwareTimer, i);
}
