/*
 * SPI_CFG.h
 *
 * Created: 12/20/2019 1:12:32 PM
 *  Author: Ahmed Alaa
 */ 


#ifndef SPI_REG_H_
#define SPI_REG_H_

/* SPI Control Register */
#define SPCR    (*(volatile uint8_t *)(0x2D))	
#define SPIE 7	/* SPI Interrupt Enable*/
#define SPE  6  /* SPI Enable */
#define DORD 5  /* Data Order */
#define MSTR 4  /* Master/Slave Select */
#define CPOL 3  /* Clock Polarity */
#define CPHA 2  /* Clock Phase */
#define SPR1 1  /* SPI Clock Rate Select 1 */
#define SPR0 0  /* SPI Clock Rate Select 0 */

/* SPI Status Register */
#define SPSR	(*(volatile uint8_t *)(0x2E))	
#define SPIF  7 /* SPI Interrupt Flag   */
#define WCOL  6 /* Write COLlision Flag */
#define SPI2X 0 /* Double SPI Speed Bit */

/* SPI Data Register */
#define SPDR	(*(volatile uint8_t *)(0x2F))	


/* Status Register */
#define SREG    (*(volatile uint8_t *)(0x5F))
#define GLOBAL_INTERRUPT_ENABLE 7

#endif /* SPI_CFG_H_ */