/*
 * APP.c
 *
 * Created: 12/18/2019 12:42:35 PM
 *  Author: ahmed
 */ 

  #include  "APP.h"
  #include "DIO.h"
  #include "UART_Pb.h"
  #include "SPI_Pb.h"
   #define F_CPU 8000000UL
   #include <util/delay.h>

  #define IDLE 0
  #define RECEIVING_BYTE 1
  #define RECEIVING_BYTE_COMPLETE 2

  static uint8_t volatile u8_UART_TX_Flag = 1 ;					/* Flag to indicate that Byte Sent successfully */
  static uint8_t  g_Received_Speed_PC1=0;						/* Data buffer for UART data Received from PC1 */
  static uint8_t volatile u8_VSE_State_Machine = IDLE ;			/* IDLE state for KIT 1 State machine */
  static uint8_t volatile Temp_Data=0;
  static uint8_t volatile g_u8_Global_Speed=0;

  /* Looping for states of VSE */
  extern void VSE_dispatcher_KIT1(void)
  {
	  switch (u8_VSE_State_Machine)
	  {
		  case IDLE :  /* Do nothing */
		  break;
		  case RECEIVING_BYTE :		/* stay here ISR will change this state */
										/* Check for button 0 to increase speed */
										if(READ_BIT(PINB,0))
										{
											_delay_ms(20);
											if(READ_BIT(PINB,0))
											{
												u8_VSE_State_Machine = RECEIVING_BYTE_COMPLETE;
											}
											while(READ_BIT(PINB,0));
										}
										/* Check for button 1 decrease speed */
										else if(READ_BIT(PINC,4))
										{
											_delay_ms(20);
											if(READ_BIT(PINC,4))
											{
												if(g_u8_Global_Speed <= 254)
												{
													g_u8_Global_Speed+=1;
												}
												else
												{
													g_u8_Global_Speed=255;
												}
											}
											while(READ_BIT(PINC,4));
										}
										else if(READ_BIT(PINB,2))
										{
											_delay_ms(20);
											if(READ_BIT(PINB,2))
											{
												if(g_u8_Global_Speed >= 1)
												{
													g_u8_Global_Speed-=1;
												}
												else
												{
													g_u8_Global_Speed=0;
												}
											}
											while(READ_BIT(PINB,2));
										}

		  break;
		  case RECEIVING_BYTE_COMPLETE :/* SPI send data to the other KIT */
										SPI_Send_Byte(g_u8_Global_Speed);
										Temp_Data=0;	/* clear Temp data for the next byte */
										u8_VSE_State_Machine = RECEIVING_BYTE;
										break;

		  default :
		  break;
	  }
  }


    
/* Callback function run when ISR of UART TX fire */
extern void g_VSE_TX_callback(void)
{
	u8_UART_TX_Flag = 1;				     /* to indicate that byte already sent and ready to sent new one */
}


/* Callback function run when ISR of UART RX fire */
extern void g_VSE_RX_callback(void)
{	
	UART_Start_RecieveByte(&g_Received_Speed_PC1);
	PORTC ^= 0xff;
	if(g_Received_Speed_PC1 == 13)
	{	
		PORTA = Temp_Data;
		g_u8_Global_Speed = Temp_Data ;
		u8_VSE_State_Machine = RECEIVING_BYTE_COMPLETE;
	}
	else if ((g_Received_Speed_PC1 != '\0')&&((g_Received_Speed_PC1 >= '0')||(g_Received_Speed_PC1 <= '9')))
	{
		Temp_Data = (Temp_Data * 10) + (g_Received_Speed_PC1-48);
		g_Received_Speed_PC1 = '\0';
	}
	else if ((g_Received_Speed_PC1 != '\0')&&(g_Received_Speed_PC1 >= 'a'))
	{
		Temp_Data = (Temp_Data * 10) + (g_Received_Speed_PC1);
		g_Received_Speed_PC1 = '\0';
	}
		
	    

}