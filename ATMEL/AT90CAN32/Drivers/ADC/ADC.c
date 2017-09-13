/* ******************************************************************
 *	ADC.c
 * ******************************************************************/

#include "GlobalDef.h"
#include "BitTwiddling.h"
#include "ADC.h"

/* ******************************************************************
 *	DEFINES
 * ******************************************************************/

/*
* @brief	Shunt down digital input buffer share by ADC channel to reduce power consumption
*/
#define Adc7_power_saving()				 DIDR0  |=(1<<ADC7D)
#define Adc6_power_saving()				 DIDR0  |=(1<<ADC6D)
#define Adc5_power_saving()				 DIDR0  |=(1<<ADC5D) //1 to disable the digital input buffer on the corresponding ADC pin.
#define Adc4_power_saving()				 DIDR0  |=(1<<ADC4D)
#define Adc3_power_saving()				 DIDR0  |=(1<<ADC3D)
#define Adc2_power_saving()				 DIDR0  |=(1<<ADC2D)
#define Adc1_power_saving()				 DIDR0  |=(1<<ADC1D)
#define Adc0_power_saving()				 DIDR0  |=(1<<ADC0D)


/*
* @brief	ADC  reference voltage
* @par1		ADC_ReferenceVoltage_X where X:  AREF, AVCC, INT_VREF
*/
#define ADC_REFERENCE_VOLTAGE_CONFIGURATION(adc_reference_voltage)\
ADMUX = ((ADMUX & ~(BIT_TWIDDLER_BIT_VALUE(REFS1) | BIT_TWIDDLER_BIT_VALUE(REFS0))) | (adc_reference_voltage<<(6)))

/*
* @brief	ADC result adjust configure
* @par1		adc_adjust_value defined as either ADC_RIGHT_ADJUST_RESULT or ADC_LEFT_ADJUST_RESULT
*/
#define ADC_ADJUST_CONFIGURATION(adc_adjust_value)\
adc_adjust_value == 1 ? (ADMUX  |=  (1<<ADLAR)) : (ADMUX  &= ~(1<<ADLAR))


/*
*@brief		enable/disable ADC module
*@par1		adc_enable_value is either ENABLE or DISABLE
*/
#define ADC_MODULE_CONFIGURATION(adc_enable_value)\
adc_enable_value == ENABLE ? (ADCSRA |=(1<<ADEN)) : (ADCSRA &=~(1<<ADEN))

/*
* @brief	ADC channel select configure
* @par1		channel
*/
#define ADC_SELECT_CHANNEL(channel) \
(ADMUX = ( ADMUX & (~((1<<MUX4)|(1<<MUX3)|(1<<MUX2)|(1<<MUX1)|(1<<MUX0)))) \
| (channel) )



/*
* @brief	ADC interrupt enable command
*/
#define ADC_INTERRUPT_ENABLE()                  (ADCSRA |=  (1<<ADIE) )

/*
* @brief	ADC interrupt disable command
*/
#define ADC_INTERRUPT_DISABLE()                 (ADCSRA &= ~(1<<ADIE) )

/*
* @brief	ADC interrupt configure
* @par1		adc_interrupt_mode can be either ENABLE or DISABLE
*/
#define ADC_INTERRUPT_CONFIGURATION(adc_interrupt_mode)\
adc_interrupt_mode == ENABLE ? ADC_INTERRUPT_ENABLE() : ADC_INTERRUPT_DISABLE()

/*
* @brief	ADC auto trigger mask bit
* @par1		adc_auto_trigger_value is defined as either ENABLE or DISABLE
*/
#define ADC_AUTO_TRIGGER_CONFIGURATION(adc_auto_trigger_value)\
adc_auto_trigger_value == ENABLE ? (ADCSRA |=(1<<ADATE)) : (ADCSRA &=~(1<<ADATE))


/*
* @brief  prompt adc to convert (or get adc measurement)
* @par1   channel is defined to be either ADC0...ADC7
*/

#define ADC_START_CONV_CHANNEL(channel)		(ADC_SELECT_CHANNEL(channel),\
ADC_START_CONV() )

/*
* @brief	ADC interrupt clear flag command
*/
#define ADC_CLEAR_FLAG()						(ADCSRA &=  (1<<ADIF) )


/*
* @brief	ADC pre-scaler configuration
* @par1		adc_prescaler is selected among the ADC pre-scaler selection ( ADC_PRESCALER_2, ADC_PRESCALER_4	, ...)
*/
#define ADC_PRESCALER_CONFIGURATION(adc_prescaler)     ADCSRA =((ADCSRA & (~((1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0))))| (adc_prescaler))



/*
* @brief	ADC trigger source configure
* @par1		adc_auto_trigger_source is defined as an option of ADC trigger source selection (ADC_FREE_RUNNING, DC_TRIGGER_WITH_ANALOG_COMPARATOR, ...)
*/
//adc_auto_trigger_source--> ADC_FREE_RUNNING, ADC_TRIGGER_WITH_[ANALOG_COMPARATOR,EXTERNAL_INTERRUPT_0,TIMER0_CTC,TIMERR0_OVERFLOW,etc. ]
#define ADC_AUTO_TRIGGER_SOURCE(adc_auto_trigger_source)	ADCSRB =((ADCSRB& (~((1<<ADTS2)|(1<<ADTS1)|(1<<ADTS0))))| (adc_auto_trigger_source))

/* ******************************************************************
 * PUBLIC FUNCTION DEFINITIONS
 * ******************************************************************/

/*
* @brief	Shunt down digital input buffer share by ADC channel to reduce power consumption
* @par1		channel can be either ADC0, ADC1, ADC2, ..., ADC7
*/
void ADC_EnterPowerSavingMode (ADC_Channel channel){
	switch (channel)
	{
		case ADC_Channel_ADC0:
		Adc0_power_saving();
		break;

		case ADC_Channel_ADC1:
		Adc1_power_saving();
		break;

		case ADC_Channel_ADC2:
		Adc2_power_saving();
		break;

		case ADC_Channel_ADC3:
		Adc3_power_saving();
		break;

		case ADC_Channel_ADC4:
		Adc4_power_saving();
		break;

		case ADC_Channel_ADC5:
		Adc5_power_saving();
		break;

		case ADC_Channel_ADC6:
		Adc6_power_saving();
		break;

		case ADC_Channel_ADC7:
		Adc7_power_saving();
		break;

		default:
		break;
	}
}

// Enable/Disable ADC module
void ADC_EnableState(EnableState State)
{
	ADC_MODULE_CONFIGURATION(State);
}

// Set ADC pre-scaler clock
void ADC_SetPrescaler(ADC_Prescaler Prescaler)
{
	ADC_PRESCALER_CONFIGURATION(Prescaler);
}

// Set ADC data register align (left/right)
void ADC_SetDataAdjust(ADC_Adjust Adjust)
{
	ADC_ADJUST_CONFIGURATION(Adjust);
}

// Set ADC reference voltage
void ADC_SetVoltageReference(ADC_ReferenceVoltage Reference)
{
	ADC_REFERENCE_VOLTAGE_CONFIGURATION(Reference);
}

// Enable/Disable ADC interrupt
void ADC_IntereruptEnable(EnableState State)
{
	ADC_INTERRUPT_CONFIGURATION(State);
}

// Enable/ Disable ADC auto trigger
void ADC_SetAutoTriggerState(EnableState State)
{
	 ADC_AUTO_TRIGGER_CONFIGURATION(State);
}

// Set ADC auto trigger source
void ADC_SetAutoTriggerSource(ADC_AutoTriggerSource Source)
{
	ADC_AUTO_TRIGGER_SOURCE(Source);

}

// Switch ADC module to ADC channel
void ADC_SelectChannel(ADC_Channel Channel)
{
	ADC_SELECT_CHANNEL(Channel);
}

/*
// Interrupt handler
ISR(ADC_vect){						// clear ADIF
	//code may be modified

}*/
