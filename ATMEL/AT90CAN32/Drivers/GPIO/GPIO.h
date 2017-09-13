//***********************************************
//	GPIO.h
//***********************************************

#include "BitTwiddling.h"

#ifndef GPIO_H_
#define GPIO_H_

/**
* @brief    Port mode selections
*/
typedef enum {
	input = 0,
	output = 1
}GPIO_PortMode;


/**
* @brief    Output logic selections
*/
typedef enum{
	high = 1,
	low = 0
}GPIO_Output;


/**
* @brief    toggle output pin
* @par1     PINx can be either PINA...G
* @par2     pin number can either 0...7
*/
#define GPIO_READ(PINx,pin_number)\
BIT_TWIDDLER_GET_BIT(PINx,pin_number)

/**
* @brief    set/reset output pin
* @par1     OutputType can be either low or high
* @par2     PORTx can be either PORTA...G
* @par3     pin number can either 0...7
*/
#define GPIO_OUTPUT(OutputType, PORTx, pin_number)\
OutputType == 1 ? BIT_TWIDDLER_SET_BIT(PORTx, pin_number): BIT_TWIDDLER_CLEAR_BIT(PORTx, pin_number)

/**
* @brief    toggle output pin
* @par1     PORTx can be either PORTA...G
* @par2     pin number can either 0...7
*/
#define GPIO_OUTPUT_TOGGLE(PORTx, pin_number)\
BIT_TWIDDLER_TOGGLE_BIT(PORTx, pin_number)

/**
* @brief    pin mode set to input or output
* @par1     PortDirectionType can be either output or input
* @par2     DDRx can be either DDRA...G
* @par3     pin number can either 0...7
*/
#define GPIO_CONFIGURATION(PortModeType, DDRx, pin_number)\
PortModeType == output ? BIT_TWIDDLER_SET_BIT(DDRx, pin_number) : BIT_TWIDDLER_CLEAR_BIT(DDRx, pin_number)
#endif /* GPIO_H_ */
