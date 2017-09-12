//***********************************************
//	ExtInterrupt.h
//***********************************************

#include "GlobalDef.h"
#include "BitTwiddling.h"

#ifndef EXT_INTERRUPT_H_
#define EXT_INTERRUPT_H_

typedef enum
{
	Ext_Interrupt_Pin_0 = 0,
	Ext_Interrupt_Pin_1 = 2,
	Ext_Interrupt_Pin_2 = 4,
	Ext_Interrupt_Pin_3 = 6,
	Ext_Interrupt_Pin_4 = 8,
	Ext_Interrupt_Pin_5 = 10,
	Ext_Interrupt_Pin_6 = 12,
	Ext_Interrupt_Pin_7 = 14
}Ext_Interrupt_Pin;

typedef enum
{
	Ext_Interrupt_Module_0 = 0,
	Ext_Interrupt_Module_1 = 1,
	Ext_Interrupt_Module_2 = 2,
	Ext_Interrupt_Module_3 = 3,
	Ext_Interrupt_Module_4 = 4,
	Ext_Interrupt_Module_5 = 5,
	Ext_Interrupt_Module_6 = 6,
	Ext_Interrupt_Module_7 = 7
}Ext_Interrupt_Module;

typedef enum
{
	Ext_Interrupt_TriggerMode_LowLevel	= 0,
	Ext_Interrupt_TriggerMode_AnyLevelChange = 1,
	Ext_Interrupt_TriggerMode_FallingEdge = 2,
	Ext_Interrupt_TriggerMode_RisingEdge = 3
}Ext_Interrupt_TriggerMode;

/*
* @brief  Set External interrupt trigger mode
* @par1   Ext_Interrupt_Pin_X where X: 0-7
* @par2   Ext_Interrupt_TriggerMode_Y where Y: LowLevel, AnyLevel, FallingEdge, RisingEdge
*/
#define  EXT_INTERRUPT_TRIGGER_MODE_CONFIGURATION(ext_interrupt_pin, mode)\
ext_interrupt_pin > 6 ? (EICRB = (EICRB & ~((BIT_TWIDDLER_BIT_VALUE(((ext_interrupt_pin)-(8))+(1))) | (BIT_TWIDDLER_BIT_VALUE((ext_interrupt_pin)-(8))))) | ((mode)<<((ext_interrupt_pin)-(8)))) : (EICRA = (EICRA & ~(BIT_TWIDDLER_BIT_VALUE((ext_interrupt_pin)+(1)) | BIT_TWIDDLER_BIT_VALUE(ext_interrupt_pin))) | ((mode)<<(ext_interrupt_pin)))

/*
* @brief  Enable/Disable External interrupt
* @par1   Ext_Interrupt_Module_X where X: 0-7
* @par2   ENABLE or DISABLE
*/
#define  EXT_INTERRUPT_MODULE_ENABLE(MODULE, ENABLE_STATE)\
ENABLE_STATE == ENABLE ? BIT_TWIDDLER_SET_BIT(EIMSK, MODULE) : BIT_TWIDDLER_CLEAR_BIT(EIMSK, MODULE)

#endif /* EXT_INTERRUPT_H_ */
