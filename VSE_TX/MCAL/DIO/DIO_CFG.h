#ifndef _DIO_CFG_H
#define _DIO_CFG_H
#include "STD_TYPES.h"

/* PORTA */
#define PORTA (*(volatile uint8_t *)(0x3B))
#define DDRA  (*(volatile uint8_t *)(0x3A))
#define PINA  (*(volatile uint8_t *)(0x39))
/* PORTB */
#define PORTB (*(volatile uint8_t *)(0x38))
#define DDRB  (*(volatile uint8_t *)(0x37))
#define PINB  (*(volatile uint8_t *)(0x36))
/* PORTC */
#define PORTC (*(volatile uint8_t *)(0x35))
#define DDRC  (*(volatile uint8_t *)(0x34))
#define PINC  (*(volatile uint8_t *)(0x33))
/* PORTD */
#define PORTD (*(volatile uint8_t *)(0x32))
#define DDRD  (*(volatile uint8_t *)(0x31))
#define PIND  (*(volatile uint8_t *)(0x30))
/* AVR PORT NUMBERS */
#define PORT_A 0
#define PORT_B 1
#define PORT_C 2
#define PORT_D 3
/* AVR PIN OUT */
#define A0 0
#define A1 1
#define A2 2
#define A3 3
#define A4 4
#define A5 5
#define A6 6
#define A7 7
#define B0 8
#define B1 9
#define B2 10
#define B3 11
#define B4 12
#define B5 13
#define B6 14
#define B7 15
#define C0 16
#define C1 17
#define C2 18
#define C3 19
#define C4 20
#define C5 21
#define C6 22
#define C7 23
#define D0 24
#define D1 25
#define D2 26
#define D3 27
#define D4 28
#define D5 29
#define D6 30
#define D7 31
#endif











