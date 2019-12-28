/*
 * TIMER_private.h
 *
 *  Created on: ??þ/??þ/????
 *      Author: Eng Mohamed Hamdy
 */

#ifndef TIMER_PRIVATE_H_
#define TIMER_PRIVATE_H_



#define TCNT0 	 *((volatile uint8_t*)0x52)
#define TCCR0	 *((volatile uint8_t*)0x53)
#define TIMSK 	 *((volatile uint8_t*)0x59)
#define SREG 	 *((volatile uint8_t*)0x5A)

#endif /* TIMER_PRIVATE_H_ */
