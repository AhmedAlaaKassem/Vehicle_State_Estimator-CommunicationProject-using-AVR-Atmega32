/*
 * TIMER_prog.c
 *
 *  Created on: ??þ/??þ/????
 *      Author: Eng Mohamed Hamdy
 */

#include "STD_TYPES.h"
#include "COMMON_MACROS.h"
#include "TIMER_interface.h"
#include "TIMER_private.h"



void TIMER_vidInit(void)
{
	CLEAR_BIT(TCCR0,7);///////
	CLEAR_BIT(TCCR0,6); // normal mode
	CLEAR_BIT(TCCR0,3);
	CLEAR_BIT(TCCR0,5); //Normal port operation, OC0 disconnected.
	CLEAR_BIT(TCCR0,4);
	SET_BIT(TCCR0,2); //clk I/O (No prescaling)
	CLEAR_BIT(TCCR0,1);
	SET_BIT(TCCR0,0);
	TCNT0=0;
	SET_BIT(TIMSK,0);
	SET_BIT(SREG,7);

}

