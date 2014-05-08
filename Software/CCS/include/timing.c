/*
 * timing.c
 *
 *  Created on: May 8, 2014
 *      Author: Keenan Johnson
 */

#include "timing.h"

//Delays a number of milliseconds
void ms_delay( int milliseconds )
{
	SysCtlDelay( milliseconds * (SysCtlClockGet() / 100) );
}
