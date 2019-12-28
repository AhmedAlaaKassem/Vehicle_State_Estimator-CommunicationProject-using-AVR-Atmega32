#ifndef TIMER_REG_H_
#define TIMER_REG_H_
#include "STD_TYPES.h"

/**____________________________________________________________**/
/**__________________ TIMER0 REGISTERS & BITS _________________**/
/* TIMER0 Control Register */
#define TIMER0_TCCR0          *((volatile uint8_t*)0x53)
#define TIMER0_TCCR0_CS00  0
#define TIMER0_TCCR0_CS01  1
#define TIMER0_TCCR0_CS02  2
#define TIMER0_TCCR0_WGM01 3
#define TIMER0_TCCR0_COM00 4
#define TIMER0_TCCR0_COM01 5
#define TIMER0_TCCR0_WGM00 6
#define TIMER0_TCCR0_FOC0  7
/* TIMER0 Interrupt Mask Register */
#define TIMER0_TIMSK           *((volatile uint8_t*)0x59)
#define TIMER0_TIMSK_TOIE0 0
#define TIMER0_TIMSK_OCIE0 1
/* TIMER0 Interrupt Flag Register */
#define TIMER0_TIFR            *((volatile uint8_t*)0x58)
#define TIMER0_TIFR_TOV0   0
#define TIMER0_TIFR_OCF0   1
/* TIMER0 Register */
#define TIMER0_TCNT0          *((volatile uint8_t*)0x52)
/* TIMER0 Output Compare Register */
#define TIMER0_OCR0           *((volatile uint8_t*)0x5C)
/**____________________________________________________________**/
/**_________________ TIMER1 REGISTERS & BITS _________________**/
/* TIMER1 Control Register */
#define TIMER1_TCCR1A  *((volatile uint8_t*)0x4F)
#define TIMER1_TCCR1A_COM1A1 7
#define TIMER1_TCCR1A_COM1A0 6
#define TIMER1_TCCR1A_COM1B1 5
#define TIMER1_TCCR1A_COM1B0 4
#define TIMER1_TCCR1A_FOC1A 3
#define TIMER1_TCCR1A_FOC1B 2
#define TIMER1_TCCR1A_WGM11 1
#define TIMER1_TCCR1A_WGM10 0
/* cont TIMER1 */
#define TIMER1_TCCR1B  *((volatile uint8_t*)0x4E)
#define TIMER1_TCCR1B_ICNC1 7
#define TIMER1_TCCR1B_ICES1 6
#define TIMER1_TCCR1B_WGM13 4
#define TIMER1_TCCR1B_WGM12 3
#define TIMER1_TCCR1B_CS12 2
#define TIMER1_TCCR1B_CS11 1
#define TIMER1_TCCR1B_CS10 0
/* TIMER1 Interrupt Mask Register */
#define TIMER1_TIMSK  *((volatile uint8_t*)0x59)
#define TIMER1_TIMSK_TICIE1 5
#define TIMER1_TIMSK_OCIE1A 4
#define TIMER1_TIMSK_OCIE1B 3
#define TIMER1_TIMSK_TOIE1 2
/* TIMER1 Interrupt Flag Register */
#define TIMER1_TIFR  *((volatile uint8_t*)0x58)
#define TIMER1_TIFR_ICF1 5
#define TIMER1_TIFR_OCF1A 4
#define TIMER1_TIFR_OCF1B 3
#define TIMER1_TIFR_TOV1 2
/* TIMER1 Register */
#define TIMER1_TCNT1   *((volatile uint16_t*)0x4C)
/* TIMER1 Output Compare Register CH1*/
#define TIMER1_OCR1A   *((volatile uint16_t*)0x4A)
/* TIMER1 Output Compare Register CH2*/
#define TIMER1_OCR1B   *((volatile uint16_t*)0x48)
/* TIMER1 Input Capture Register */
#define TIMER1_ICR1    *((volatile uint16_t*)0x46)
/**____________________________________________________________**/
/**__________________ TIMER2 REGISTERS & BITS _________________**/
/* TIMER2 Control Register */
#define TIMER2_TCCR2          *((volatile uint8_t*)0x45)
#define TIMER2_TCCR2_CS20  0
#define TIMER2_TCCR2_CS21  1
#define TIMER2_TCCR2_CS22  2
#define TIMER2_TCCR2_WGM21 3
#define TIMER2_TCCR2_COM20 4
#define TIMER2_TCCR2_COM21 5
#define TIMER2_TCCR2_WGM20 6
#define TIMER2_TCCR2_FOC2  7
/* TIMER0 Interrupt Mask Register */
#define TIMER2_TIMSK           *((volatile uint8_t*)0x59)
#define TIMER2_TIMSK_TOIE2 6
#define TIMER2_TIMSK_OCIE2 7
/* TIMER0 Interrupt Flag Register */
#define TIMER2_TIFR            *((volatile uint8_t*)0x58)
#define TIMER2_TIFR_TOV2   6
#define TIMER2_TIFR_OCF2   7
/* TIMER0 Register */
#define TIMER2_TCNT2          *((volatile uint8_t*)0x44)
/* TIMER0 Output Compare Register */
#define TIMER2_OCR2           *((volatile uint8_t*)0x43)
/* TIMER2 Asynchronous Status Register */
#define TIMER2_ASSR           *((volatile uint8_t*)0x42) 
#define TIMER2_TCR2UB 0
#define TIMER2_OCR2UB 1
#define TIMER2_TCN2UB 2
#define TIMER2_AS2    3
/**____________________________________________________________**/
/* Status Register */
#define SREG                   *((volatile uint8_t*)0x5F)
#define GLOBAL_INTERRUPT_ENABLE 7
/**____________________________________________________________**/
#endif /* TIMER_REG_H_ */
