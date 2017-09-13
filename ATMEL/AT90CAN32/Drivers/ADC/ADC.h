/* ******************************************************************
 *	ADC.h
 * ******************************************************************/

#include "GlobalDef.h"
#include "BitTwiddling.h"
#include <avr/io.h>

#ifndef ADC_H_
#define ADC_H_

/* ******************************************************************
 *	TYPES
 * ******************************************************************/

/*
* @brief	ADC Prescaler
*/
typedef enum
{
	ADC_Prescaler_1	= 0,
	ADC_Prescaler_2	= 1,
	ADC_Prescaler_4	= 2,
	ADC_Prescaler_8	= 3,
	ADC_Prescaler_16 = 4,
	ADC_Prescaler_32 = 5,
	ADC_Prescaler_64 = 6,
	ADC_Prescaler_128 = 7
}ADC_Prescaler;

/*
* @brief	ADC channel selection
*/
typedef enum
{
	ADC_Channel_ADC0		= 0,
	ADC_Channel_ADC1		= 1,
	ADC_Channel_ADC2		= 2,
	ADC_Channel_ADC3		= 3,
	ADC_Channel_ADC4		= 4,
	ADC_Channel_ADC5		= 5,
	ADC_Channel_ADC6		= 6,
	ADC_Channel_ADC7		= 7,
	ADC_Channel_BANDGAP_1V1	= 30,
	ADC_Channel_GND			= 31
}ADC_Channel;

/*
* @brief	ADC reference voltage selection
*/
typedef enum
{
	ADC_ReferenceVoltage_AREF = 0,
	ADC_ReferenceVoltage_AVCC = 1,
	ADC_ReferenceVoltage_INT_VREF = 3
}ADC_ReferenceVoltage;

/*
* @brief	ADC data adjust (left/right)
*/
typedef enum
{
	ADC_Adjust_Right = 0,
	ADC_Adjust_Left = 1
}ADC_Adjust;


/*
* @brief	ADC trigger source selection
*/
typedef enum
{
	ADC_AutoTriggerSource_FreeRunning  =  0,
	ADC_AutoTriggerSource_AnalogComparator =  1,
	ADC_AutoTriggerSource_ExternalInterrupt0 =  2,
	ADC_AutoTriggerSource_Timer0CTC =  3,
	ADC_AutoTriggerSource_Timer0Overflow =  4,
	ADC_AutoTriggerSource_Timer1CTCB =  5,
	ADC_AutoTriggerSource_Timer1Overflow =  6,
	ADC_AutoTriggerSource_Timer1CaptureEvent = 7
}ADC_AutoTriggerSource;

/*
* @brief	with the assumption that adc value is right adjusted, get the ADC result in 8 bits or 10 bits
*/
#define ADC_GET_8_BITS_RESULT()          ((U8)(ADCH))
#define ADC_GET_10_BITS_RESULT()         ((U16)(ADCL+((U16)(ADCH<<8))))

/*
* @brief	ADC interrupt clear flag command
*/
#define ADC_CLEAR_FLAG()				(ADCSRA &=  (1<<ADIF) )

/*
* @brief	ADC clear mux
*/
#define ADC_CLEAR_MUX()					(ADMUX  &= ~((1<<MUX4)|(1<<MUX3)|(1<<MUX2)|(1<<MUX1)|(1<<MUX0)) )

/*
* @brief  prompt adc to convert (or get adc measurement)
* @note   ADSC set to logic high triggers ADC module to convert. Upon conversion completion, ADSC will be reset to zero by hardware
* @note2  with auto trigger disable, user must set ADSC to logic 1 whenever conversion is required. In free running mode, witting ADSC to logic high will mark the start of auto conversion
*/
#define ADC_START_CONVERSION()			ADCSRA |= (1<<ADSC)


/* ******************************************************************
 * PUBLIC FUNCTION DECLARATIONS
 * ******************************************************************/
void ADC_SetDataAdjust(ADC_Adjust Adjust);
void ADC_SetPrescaler(ADC_Prescaler Prescaler);
void ADC_IntereruptEnable(EnableState State);
void ADC_SetVoltageReference(ADC_ReferenceVoltage Reference);
void ADC_EnterPowerSavingMode(ADC_Channel channel);
void ADC_SetAutoTriggerSource(ADC_AutoTriggerSource Source);
void ADC_SetAutoTriggerState(EnableState State);
void ADC_EnableState(EnableState State);
void ADC_SelectChannel(ADC_Channel Channel);

#endif /* ADC_H_ */
