//*************************
// BCD.c
//*************************

#include "BCD.h"

/* *************************************************************************
 * DEFINES
 * *************************************************************************
 */
 
// Hour range 
#define HOUR_ONES_MIN	0
#define HOUR_ONES_MAX	9
#define HOUR_TENS_MIN	0
#define HOUR_TENS_MAX	2

// Minute range 
#define MINUTE_ONES_MIN	0
#define MINUTE_ONES_MAX	9
#define MINUTE_TENS_MIN	0
#define MINUTE_TENS_MAX	5

// Second range
#define SECOND_ONES_MIN	0
#define SECOND_ONES_MAX	9 
#define SECOND_TENS_MIN	0
#define SECOND_TENS_MAX	5

/* *************************************************************************
 * PRIVATE VARIABLES
 * *************************************************************************
 */
 
static U8 second_ones_counter = SECOND_ONES_MIN; 
static U8 second_tens_counter = SECOND_TENS_MIN; 
static U8 minute_ones_counter = MINUTE_ONES_MIN;  
static U8 minute_tens_counter = MINUTE_TENS_MIN; 
static U8 hour_ones_counter = HOUR_ONES_MIN;  
static U8 hour_tens_counter = HOUR_TENS_MIN;

/* *************************************************************************
 * PUBLIC FUNCTIONS DEFINITIONS
 * *************************************************************************
 */
 
/*! \fn U32 getTime(void)
 *  \brief 
 *  This function get time display time in BCD format as an U32 word of xx hh mm ss 
 *  where hh: hour (0 to 12) ;mm: minutes (0 to 59) ;ss: seconds (0 to 59)
 */
U32 BCD_GetTime(void)
{
	U32 time = 0; 
	time += (U32)second_ones_counter; 
	time += (U32)(second_tens_counter << 4);
	time += (U32)(minute_ones_counter << 8);   
	time += (U32)(minute_tens_counter << 12); 
	time += (U32)(hour_ones_counter << 16);   
	time += (U32)(hour_tens_counter << 20);  
	
	return time; 
}

/*! \fn U8 getTime_Second(void)
 *  \brief 
 *  This function returns second portion from getTime
 */
U8 BCD_GetTime_Second(void)
{
	return (U8) (BCD_GetTime());
}

/*! \fn U8 getTime_Minute(void)
 *  \brief 
 *  This function returns minute portion from getTime
 */
U8 BCD_GetTime_Minute(void)
{
	return (U8) (BCD_GetTime() >> 8);
}

/*! \fn U8 getTime_Hour(void)
 *  \brief 
 *  This function returns hour portion from getTime
 */
U8 BCD_GetTime_Hour(void)
{
	return (U8) (BCD_GetTime() >> 16); 
}

/*! \fn void resetTime(void)
 *  \brief 
 *  This function reset display time in BCD format to  hh: 00 mm: 00 ss: 00 
 *  where hh: hour (0 to 12) ;mm: minutes (0 to 59) ;ss: seconds (0 to 59)
 */
void BCD_ResetTime(void)
{
	second_ones_counter = SECOND_ONES_MIN; 
	second_tens_counter = SECOND_TENS_MIN; 
	minute_ones_counter = MINUTE_ONES_MIN;  
	minute_tens_counter = MINUTE_TENS_MIN; 
	hour_ones_counter = HOUR_ONES_MIN;  
	hour_tens_counter = HOUR_TENS_MIN;
}

/*! \fn void updateTime(void)
 *  \brief 
 *  This function display time in BCD format as an U32 word of xx hh mm ss 
 *  where xx: 0x00 ;hh: hour (0 to 12) ;mm: minutes (0 to 59) ;ss: seconds (0 to 59)
 *  \note 
 *  call this function every second to update time
 */
void BCD_UpdateTime(void)
{
	// Level 1
	second_ones_counter++;
	if (second_ones_counter > SECOND_ONES_MAX)
	{
		second_ones_counter = SECOND_ONES_MIN; 
		
		// Level 2
		second_tens_counter++;
		if (second_tens_counter > SECOND_TENS_MAX)
		{
			second_tens_counter = SECOND_TENS_MIN;
			
			// Level 3
			minute_ones_counter++;
			if (minute_ones_counter > MINUTE_ONES_MAX)
			{
				minute_ones_counter = MINUTE_ONES_MIN; 
				
				// Level 4 
				minute_tens_counter++;
				if (minute_tens_counter > MINUTE_TENS_MAX)
				{
					minute_tens_counter = MINUTE_TENS_MIN;

					// Level 5 
					hour_ones_counter++;
					
					if ((hour_tens_counter == 2) && (hour_ones_counter == 3))
					{
						// Reset time @ 23:59:59  
						BCD_ResetTime(); 
					}
						
					if (hour_ones_counter > HOUR_ONES_MAX)
					{
						hour_ones_counter = HOUR_ONES_MIN;
						

						
						// Level 6 
						hour_tens_counter++;
						if (hour_tens_counter > HOUR_TENS_MAX)
						{
							hour_tens_counter = HOUR_TENS_MIN; 
							
							
						}
					}
				}
			}
		}
	}
}
