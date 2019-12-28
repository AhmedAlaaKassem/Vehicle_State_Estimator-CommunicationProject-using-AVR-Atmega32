/*
 * UART_prog.c
 *
 *  Created on: ??þ/??þ/????
 *      Author: Eng Mohamed Hamdy
 */

#include "STD_TYPES.h"
#include "COMMON_MACROS.h"
#include "UART.h"
#include "UART_REG.h"
#include "DIO.h"
#include "Interrupt.h"
#include <avr/interrupt.h>

/*
 *	Static variable store UART mode configuration "Polling or interrupt"
 */
static uint8_t u8_UART_InterruptMode = DUMMY ;

/* Call back functions for TX & RX of UART */
static void (*callBack_tx)(void)= NULL_PTR;
static void (*callBack_rx)(void)= NULL_PTR;




/***************************************************************************************/
/* brief!  UART initialization                                                         */
/* param!  void                                                                        */
/* return! u8 Indication of the execution if it fails or not                           */
/***************************************************************************************/
uint8_t UART_Init(Str_UARTConfiguration_t *pstr_UART_cfg)
{
	/*
	 *	Development Error Tracer
	 */

	uint8_t au8_DET = ERROR_OK ;
	if(NULL != pstr_UART_cfg)
	{
		/*
		*	Checking the desired Mode if it's polling or interrupt
		*/
		if(INTERRUPT == pstr_UART_cfg -> u8_Interrupt_Mode)
		{
			Enable_GlobalInterrupt();

			SET_BIT(UCSRB, RXCIE); /* Enable RX Complete Interrupt */
			SET_BIT(UCSRB, TXCIE); /* Enable RT Complete Interrupt */
			//SET_BIT(UCSRB, UDRIE); /* Enable Data Register Empty Interrupt */
			u8_UART_InterruptMode = INTERRUPT ;
		}
		else if(POLLING == pstr_UART_cfg -> u8_Interrupt_Mode)
		{
			CLEAR_BIT(UCSRB, RXCIE); /* Disable RX Complete Interrupt */
			CLEAR_BIT(UCSRB, TXCIE); /* Disable RT Complete Interrupt */
			CLEAR_BIT(UCSRB, UDRIE); /* Disable Data Register Empty Interrupt */
			u8_UART_InterruptMode = POLLING ;
		}
		else
		{
			/* Handling of entering wrong Mode */
			au8_DET = E_INTERRUPT_MODE ;
		}

		 /*
		  * selects between accessing the UCSRC or the UBRRH Register
		  */
		SET_BIT(UCSRC, URSEL);

		/* Handling synchronization Mode */
		if(ASYNCHRONOUS == pstr_UART_cfg -> u8_Sync)
		{
			/* "Asynchronous" USART Mode Select */
			CLEAR_BIT(UCSRC, UMSEL);
			if(BAUDRATE_9600 == pstr_UART_cfg -> u16_BaudRate)
			{
				//UCSRC=0b0....... Lw 3aez ast5dm UBRRH, menf3sh a3mlo b CLEAR_BIT 3san htd5ol t2ra al awl w b3den t3ml clear f htla2y fi al bit '1'
				UBRRL = BAUDRATE_9600;//Baud Rate 9600
			}
			else
			{
				/* Handling of entering wrong BaudRate */
				au8_DET = E_BAUDRATE ;
			}

		}
		else if(SYNCHRONOUS == pstr_UART_cfg -> u8_Sync)
		{
			/* "Synchronous" USART Mode Select */
			SET_BIT(UCSRC, UMSEL);
			/*
			 * TODO
			 */
		}
		else
		{
			/* Handling of entering wrong synchronization Mode */
			au8_DET = E_SYNC ;
		}

		/*
		 * Parity Mode
		 */
		if(Disabled_PARITY == pstr_UART_cfg -> u8_Parity)
		{
			CLEAR_BIT(UCSRC, UPM1);
			CLEAR_BIT(UCSRC, UPM0);
		}
		else if(Disabled_PARITY == pstr_UART_cfg -> u8_Parity)
		{
			CLEAR_BIT(UCSRC, UPM1);
			SET_BIT(UCSRC, UPM0);
		}
		else if(Disabled_PARITY == pstr_UART_cfg -> u8_Parity)
		{
			SET_BIT(UCSRC, UPM1);
			SET_BIT(UCSRC, UPM0);
		}
		else
		{
			/* Handling of entering wrong Parity */
			au8_DET = E_PARITY ;
		}

		/*
		 *  Stop Bit Select
		 */
		if(STOP_1B == pstr_UART_cfg -> u8_StopSize)
		{
			CLEAR_BIT(UCSRC, USBS); /* One Stop Bit */
		}
		else if((STOP_2B == pstr_UART_cfg -> u8_StopSize))
		{
			SET_BIT(UCSRC, USBS); /* Two Stop Bit */
		}
		else
		{
			/* Handling of entering wrong Stop size */
			au8_DET = E_STOP_SIZE ;
		}

		if(CHAR_5B == pstr_UART_cfg -> u8_CharSize)
		{
			/* Character Size */
			SET_MASK(UCSRC, CHAR_5B);
			CLEAR_BIT(UCSRB, UCSZ2);
		}
		else if(CHAR_6B == pstr_UART_cfg -> u8_CharSize)
		{
			/* Character Size */
			SET_MASK(UCSRC, CHAR_6B);
			CLEAR_BIT(UCSRB, UCSZ2);
		}
		else if(CHAR_7B == pstr_UART_cfg -> u8_CharSize)
		{
			/* Character Size */
			SET_MASK(UCSRC, CHAR_7B);
			CLEAR_BIT(UCSRB, UCSZ2);
		}
		else if(CHAR_8B == pstr_UART_cfg -> u8_CharSize)
		{
			/* Character Size */
			//SET_MASK(UCSRC, CHAR_8B);
			//CLEAR_BIT(UCSRB, UCSZ2);
			UCSRC |= (1<< UCSZ1) | (1<< UCSZ0) | (1 << URSEL);
		}
		else if(CHAR_9B == pstr_UART_cfg -> u8_CharSize)
		{
			/* Character Size */
			SET_MASK(UCSRC, CHAR_8B);
			SET_BIT(UCSRB, UCSZ2);
		}
		else
		{
			/* Handling of entering wrong Char size */
			au8_DET = E_CHAR_SIZE ;
		}
		SET_BIT(UCSRB, RXEN); /* RX enable */
		SET_BIT(UCSRB, TXEN); /* TX enable */
	}
	else
	{
		/* Handling of entering null pointer*/
		au8_DET = E_PTR_IS_NULL ;
	}
	return au8_DET;
}

/***************************************************************************************/
/* brief!  UART Send Data                                                              */
/* param!  (I/P) Data                                                                  */
/* return! u8 Indication of the execution if it fails or not                           */
/***************************************************************************************/
uint8_t UART_Send_Byte(uint8_t au8_TXbyatCpy)
{
	/*
	 *	Development Error Tracer
	 */
	uint8_t au8_DET = ERROR_OK ;
	if(POLLING == u8_UART_InterruptMode)
	{
		while(GET_BIT(UCSRA, UDRE) == NULL);
		UDR = au8_TXbyatCpy;
	}
	else if(INTERRUPT == u8_UART_InterruptMode)
	{
			UDR = au8_TXbyatCpy;
	}
	else
	{
		/* Handling of entering wrong Mode */
		au8_DET = E_INTERRUPT_MODE ;
	}

	return au8_DET;
}

/***************************************************************************************/
/* brief!  UART Recive Data                                                            */
/* param!  (O/P) pointer to data                                                       */
/* return! u8 Indication of the execution if it fails or not                           */
/***************************************************************************************/
uint8_t UART_Start_RecieveByte(uint8_t * au8_RXbyatCpy)
{
	/*
	 *	Development Error Tracer
	 */
	uint8_t au8_DET = ERROR_OK ;
	if(NULL != au8_RXbyatCpy)
	{
		if(POLLING == u8_UART_InterruptMode)
		{
			while(GET_BIT(UCSRA, RXC) == NULL);
			*au8_RXbyatCpy = UDR;
		}
		else if(INTERRUPT == u8_UART_InterruptMode)
		{
				*au8_RXbyatCpy = UDR;
		}
		else
		{
			/* Handling of entering wrong Mode */
			au8_DET = E_INTERRUPT_MODE ;
		}
	}
	else
	{
		/* Handling of entering null pointer*/
		au8_DET = E_PTR_IS_NULL ;
	}

	return au8_DET;

}





/* Global UART TX set callback function */
uint8_t Set_Call_Back_UART_TX(void (*App)(void))
{
	uint8_t retval = ERROR_OK;
	if( App != NULL_PTR )
	{
		callBack_tx = App;
	}
	else
	{
		retval = E_PTR_IS_NULL ;
	}
	return retval;
}

/* Global UART RX set callback function */
uint8_t Set_Call_Back_UART_RX(void (*App)(void))
{
	uint8_t retval = ERROR_OK;
	if( App != NULL_PTR )
	{
		callBack_rx = App;
	}
	else
	{
		retval = E_PTR_IS_NULL ;
	}
	return retval;
}


 ISR(USART_RXC_vect)
 {
 	if(callBack_rx != NULL_PTR)
 	{
	 	callBack_rx();
 	}
 }


 ISR(USART_TXC_vect)
 {
	if(callBack_tx != NULL_PTR)
	{
		callBack_tx();
	}

 }