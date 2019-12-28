/*
 * APP.c
 *
 * Created: 12/18/2019 12:42:35 PM
 *  Author: ahmed
 */ 
#include "DIO.h"
#include "TIMER_interface.h"
#include "SPI_Pb.h"
#include "UART_Pb.h"
#define F_CPU 8000000UL
#include <util/delay.h>
#include <avr/interrupt.h>
   
#define IDLE 0
#define RECEIVING_BYTE 1
#define VSE_READ_SPI 2
#define VSE_SEND_UART 3
  
static uint8_t volatile u8_UART_TX_Flag = 1 ;					/* Flag to indicate that Byte Sent successfully */
static uint8_t OPA =0;
static volatile float32_t f_Distance=0 ;
static uint8_t volatile u8_VSE_State_Machine = RECEIVING_BYTE ;	
static volatile uint32_t OVF_counter=0;

extern void VSE_dispatcher_KIT2(void)
{
	switch (u8_VSE_State_Machine)
	{
		case IDLE :
		break;
		case RECEIVING_BYTE :	/*check for button 0  fetching data */
								if(READ_BIT(PINC,4))
								{
									_delay_ms(20);
									if(READ_BIT(PINC,4))
									{
										DIO_Write_Pin(B0,HIGH);
										_delay_ms(100);
										DIO_Write_Pin(B0,LOW);
									}
									while(READ_BIT(PINC,4));
								}
								/* Check for button 1 displaying data on PC2 */
								else if(READ_BIT(PINB,2))
								{
									_delay_ms(20);
									if(READ_BIT(PINB,2))
									{
										/* change stat to send data to UART */
										u8_VSE_State_Machine = VSE_SEND_UART;
									}
									while(READ_BIT(PINB,2));
								}
		break;
		case VSE_READ_SPI :				/* Start Timer count */
										TIMER_vidInit();
										u8_VSE_State_Machine = RECEIVING_BYTE;
		break;
		case VSE_SEND_UART :		    /* restart timer */
										/* calculatee the distance time * OPA */ 
										f_Distance = (float32_t) ((TCNT0 + (OVF_counter* 256))*(float32_t)((float32_t)1024/F_CPU));
										f_Distance *= (float32_t)OPA;
										/* Send distance to UART */
										UART_Send_Byte((uint8_t)f_Distance);      /* send data to PC by UART */
										/* clear Timer */
										TCNT0=0;
										OVF_counter=0;
										u8_VSE_State_Machine = RECEIVING_BYTE;
		break;
		default:
		break;
	}
}

/* call back function ISR OF SPI */
void g_VSE_TX_callbackSPI(void)
{
	SPI_Receive_Byte(&OPA);
	u8_VSE_State_Machine = VSE_READ_SPI;
}

/* Callback function run when ISR of UART TX fire */
extern void g_VSE_TX_callback(void)
{
	u8_UART_TX_Flag = 1;				     /* to indicate that byte already sent and ready to sent new one */
}

/* timer 0 OverFlow ISR */
ISR(TIMER0_OVF_vect)
{
	OVF_counter ++;

}