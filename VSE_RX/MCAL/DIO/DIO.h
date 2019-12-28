#ifndef _DIO_H
#define _DIO_H

#include "STD_TYPES.h"
#include "DIO_CFG.h"
#include "COMMON_MACROS.h"

/* Pin Vales */
#define HIGH   1
#define LOW    0
/* Pin Direction */
#define OUT    1
#define IN     0
/* PORT Vales */
#define PORT_LOW  0x00
#define PORT_HIGH 0xFF
/* PORT Direction */
#define INPUTS  0x00
#define OUTPUTS 0xFF
/* Error */
#define ERROR_OK  0
#define ERROR_NOK 1

/* Used Macros */
#define AVR_PORT_BITS 8

uint8_t  DIO_Init_Pin  (uint8_t u8_Pin_Number ,uint8_t u8_Pin_State);
uint8_t  DIO_Init_Port (uint8_t u8_Port_Number,uint8_t u8_Port_State);
uint8_t  DIO_Write_Pin (uint8_t u8_Pin_Number,uint8_t Pin_Value);
uint8_t  DIO_Read_Pin  (uint8_t u8_Pin_Number,uint8_t *pu8_Reading);
uint8_t  DIO_Write_Port(uint8_t u8_Port_Number,uint8_t Port_Value);
uint8_t  DIO_Read_Port (uint8_t u8_Port_Number,uint8_t *pu8_Reading);



#endif
