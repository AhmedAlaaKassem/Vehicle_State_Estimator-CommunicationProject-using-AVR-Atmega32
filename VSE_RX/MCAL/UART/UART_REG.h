/*
 * UART_private.h
 *
 *  Created on: ??þ/??þ/????
 *      Author: Eng Mohamed Hamdy
 */

#ifndef __UART_REG_H__
#define __UART_REG_H__

/* UCSRA */
#define UCSRA 		*((volatile uint8_t*) 0x2BU)
#define RXC     7U
#define TXC     6U
#define UDRE    5U
#define FE      4U
#define DOR     3U
#define PE      2U
#define U2X     1U
#define MPCM    0U

/* UCSRB */
#define	UCSRB		*((volatile uint8_t*) 0x2AU)
#define RXCIE   7U
#define TXCIE   6U
#define UDRIE   5U
#define RXEN    4U
#define TXEN    3U
#define UCSZ2   2U
#define RXB8    1U
#define TXB8    0U

/* UCSRC */
#define UCSRC 		*((volatile uint8_t*) 0x40U)
#define URSEL   7U
#define UMSEL   6U
#define UPM1    5U
#define UPM0    4U
#define USBS    3U
#define UCSZ1   2U
#define UCSZ0   1U
#define UCPOL   0U

#define UBRRH		*((volatile uint8_t*) 0x40U)
#define UBRRL		*((volatile uint8_t*) 0x29U)
#define UDR 		*((volatile uint8_t*) 0x2CU)





#endif /* __UART_PRIVATE_H__ */
