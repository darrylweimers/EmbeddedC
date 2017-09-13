//***********************************************
//	Example_ADC.c
//***********************************************

#include "Example_ADC.h"
#include "GPIO.h"

/* *************************************************************************
 * PUBLIC FUNCTION DEFINITIONS
 * *************************************************************************
 */
/*
* @brief	with the assumption that ADC result is right alight, this function reads the approximated digital value from a specific ADC channel
* @par1		ADC channel defines as either ADC0...7
* @par2		sampling number
*/
U16 ADC_Read(ADC_Channel Channel)
{
	ADC_SelectChannel(Channel);
	ADC_START_CONVERSION();
	while (!(ADCSRA & (1<<ADIF)));    // wait until conversion is completed or interrupt flag set by hardware once conversion has been completed
	return ADC_GET_10_BITS_RESULT();
}

/*
* @brief	example in configuring ADC module
*/
void ADC_Init(void)
{
	GPIO_CONFIGURATION(input, DDRF, 0);   //ADC0 set to input mode
	GPIO_CONFIGURATION(input, DDRF, 1);   //ADC1 set to input mode
	GPIO_CONFIGURATION(input, DDRF, 2);   //ADC2 set to input mode
	GPIO_CONFIGURATION(input, DDRF, 3);   //ADC3 set to input mode
	GPIO_CONFIGURATION(input, DDRF, 4);   //ADC4 set to input mode
	GPIO_CONFIGURATION(input, DDRF, 5);   //ADC5 set to input mode
	GPIO_CONFIGURATION(input, DDRF, 6);   //ADC6 set to input mode
	GPIO_CONFIGURATION(input, DDRF, 7);   //ADC7 set to input mode

	ADC_EnableState(ENABLE);
	ADC_SetPrescaler(ADC_Prescaler_4);
	ADC_CLEAR_MUX();
	ADC_SetDataAdjust(ADC_Adjust_Right);
	ADC_SetVoltageReference(ADC_ReferenceVoltage_INT_VREF);
	ADC_IntereruptEnable(DISABLE);
	ADC_SetAutoTriggerState(DISABLE);
}
