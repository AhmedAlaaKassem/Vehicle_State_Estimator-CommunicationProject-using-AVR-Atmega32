/*
 * BCM.c
 *
 * Created: 12/23/2019 11:19:19 PM
 *  Author: ahmed
 */ 
 #include "BCM.h"
 #include "UART_Pb.h"
 #include "SPI_Pb.h"

/* ________________________________________________LOCAL MACROS_______________________________________________ */

 /* BCM TX MACROS */
 #define BCM_FIRST_BYTE           0            /* First Byte in BCM Frame */
 #define BCM_SECOND_BYTE          1            /* Second Byte in BCM Frame  "Data Size" MOST  */
 #define BCM_THIRD_BYTE           2            /* Second Byte in BCM Frame  "Data Size" LEAST */
 #define LAST_BYTE_TX  (str_BCM_Fram_TX.u16_Data_Size)+3                   /* Last Byte in Frame */
 #define LAST_BYTE_RX  (str_BCM_Fram_RX.u16_Data_Size)+3
 /* BCM_Lock */
 #define UNLOCKED 0
 #define LOCKED   1
 /* BCM TX States */
 #define IDLE                     0
 #define SENDING_BYTE             1
 #define SENDING_BYTE_COMPLETE    2
 #define SENDING_FRAME_COMPLETE   3
 /* BCM RX States */
 #define RECEIVING_BYTE           1
 #define RECEIVING_BYTE_COMPLETE  2
 #define RECEIVING_FRAME_COMPLETE 3
 /* Sending Byte states */
 #define BYTE_IS_SENT 		      1
 #define SENDING 			      0
 /* RX Buffer setup */
 #define SETUP_SUCCEEDED          0

 /*_____________________________________________________________________________________________________________*/

 /* _____________________________________________LOCAL Datatypes_______________________________________________ */

 /* Local Structure for BCM frame */ 
 typedef struct {
	 uint8_t   u8_BCM_ID;	      /* BCM ID */
	 uint16_t  u16_Data_Size;     /* Size of data in tx Buffer */
	 uint8_t * pu8_Data_Address;  /* pointer to buffer address "Data address" */
	 uint8_t   u8_Check_Sum;      /* Check sum for Data transmitted */
 }str_BCM_Fram;

 str_BCM_Fram str_BCM_Fram_TX;   /* Object for TX BCM Frame */
 str_BCM_Fram str_BCM_Fram_RX;   /* object for RX BCM Frame */

 /*_____________________________________________________________________________________________________________*/
	
 /* __________________________________________LOCAL FUNCTIONS PROTOTYPES_______________________________________ */

 /* Static local functions run when global set back function called */
 static void(*g_callBack_tx)(void);
 static void(*g_callBack_rx)(void);

 /*_____________________________________________________________________________________________________________*/

/* ____________________________________________GLOBAL STATIC VARIABLES_________________________________________ */

 static  uint8_t volatile u8_UART_TX_Flag = BYTE_IS_SENT ;	/* Flag to indicate that Byte Sent successfully */
 static  uint8_t gu8_BCM_init = BCM_NOT_INITIALIZED;        /* Global variable to indicate BCM TX Initialized or not */
 static  uint8_t gu8_lock_TX_Buffer = UNLOCKED ;			/* Global variable to set Lock on BCM_TX Buffer */
 static  uint8_t gu8_lock_RX_Buffer = UNLOCKED ;			/* Global variable to set Lock on BCM_RX Buffer */
 static  uint8_t gu8_setupRX_Buffer = SETUP_FAILED ;		/* BCM RX Buffer initial setup not initialized */
 static  uint32_t gu8_BCM_Index_Buffer_TX_Dispat = 0;		/* Index counter for TX BCM Dispatcher */
 static  uint32_t gu8_BCM_Index_Buffer_RX_Dispat = 0;		/* Index counter for RX BCM Dispatcher */
 static  volatile uint32_t gu8_BCM_RX_Index_Buffer_ISR = 0;	/* Index counter for ISR RX of BCM Module */
 static  volatile uint8_t gu8_BCM_State_Machine_TX = IDLE ; /* State Machine for TX BCM */
 static  volatile uint8_t gu8_BCM_State_Machine_RX = IDLE ; /* State Machine for RX BCM */
 static  uint8_t gu8_BCM_RX_data =0;						/* global Buffer to receive data from UART RX */

 /*_____________________________________________________________________________________________________________*/

 /* ___________________________________ LOCAL FUNCTIONS IMPLEMENTATION _________________________________________*/

 /* Init BCM module */
 extern uint8_t BCM_Init(uint8_t u8_Comm_Protocol)
 {
	uint8_t retval = ERROR_OK;
	switch(u8_Comm_Protocol)
		{		
			/* Check for Comm. protocol used */
			case UART : gu8_BCM_init = UART_Init(&obj_UART);
						if(gu8_BCM_init == ERROR_OK)
						{
							Set_Call_Back_UART_TX(g_BCM_TX_callback);	/* set the function that will run when UART TX ISR fire */
							Set_Call_Back_UART_RX(g_BCM_RX_callback);   /* set the function that will run when UART RX ISR fire */
							gu8_lock_TX_Buffer = UNLOCKED ;			    /* Global variable to set Lock on BCM_TX Buffer */
							gu8_lock_RX_Buffer = UNLOCKED ;			    /* Global variable to set Lock on BCM_RX Buffer */
							gu8_BCM_State_Machine_TX = IDLE ;           /* State Machine for TX BCM */
							gu8_BCM_State_Machine_RX = IDLE ;           /* State Machine for RX BCM */
							gu8_BCM_Index_Buffer_TX_Dispat = 0;		    /* Index counter for TX BCM Dispatcher */
							gu8_BCM_Index_Buffer_RX_Dispat = 0;		    /* Index counter for RX BCM Dispatcher */
							gu8_BCM_RX_Index_Buffer_ISR    = 0;	        /* Index counter for ISR RX of BCM Module */
						}
						else
						{
							retval = ERROR_UART_NOT_INITIALIZED ;
						}
							
			break;
			case SPI  : gu8_BCM_init = SPI_Init(&SPI_TX);
						/* needs to set call back function for SPI */
			break;
			default: retval = ERROR_UNDEFINED_COMM_PROTOCOL;		/* BCM didn't use UART or SPI */
			break;
		}
	return retval;
 }


 /* BCM send change state to sending byte - start transmission */
 extern uint8_t BCM_Send(uint16_t u16_Data_Size ,uint8_t* pu8_Data_Address)
 {	
	uint8_t retval = ERROR_OK;
	/* Check BCM Module initialized or not */
	if ( gu8_BCM_init == ERROR_OK )
	{
		/* Check for IDLE state */
		if( gu8_BCM_State_Machine_TX == IDLE )
		{	
			/* check for Null pointer */
			if( pu8_Data_Address != NULL_PTR )
			{
				/* Check if Buffer is LOCKED or not */
				if( gu8_lock_TX_Buffer == UNLOCKED )
				{
					str_BCM_Fram_TX.u8_BCM_ID = BCM_ID;				       /* assign the ID of BCM frame */
					str_BCM_Fram_TX.u16_Data_Size = u16_Data_Size;         /* assign data size to BCM frame */
					str_BCM_Fram_TX.pu8_Data_Address = pu8_Data_Address;   /* assign pointer of data BCM frame */
					str_BCM_Fram_TX.u8_Check_Sum = 0;					   /* Initial value of check sum equal zero */
					gu8_BCM_State_Machine_TX = SENDING_BYTE ;              /* Change state from idle to sending byte */
					gu8_lock_TX_Buffer = LOCKED ;						   /* Lock BCM to prevent new requests */
				}
				else
				{
					retval = ERROR_BCM_LOCKED;						       /* BCM Locked , can't request unless unlocked */
				}
			}
			else
			{
				retval = ERROR_NULL_POINTER ;
			}
		}
		else
		{
			retval = BCM_NOT_INITIALIZED;						/* BCM must start with IDLE state */
		}
	}
	else
	{
		retval = BCM_NOT_INITIALIZED;							/* BCM must initialized first */
	}
	return retval;
 }

 /* BCM TX Dispatcher to switch between states and handle sending data */
 extern uint8_t BCM_TxDispatcher(void)
 {
	uint8_t retval = ERROR_OK;
	switch (gu8_BCM_State_Machine_TX)
	{
		/* State 0 Idle state */
		case IDLE :			 /* DO Nothing you're in rest here */
		break;
		/*  State 1 Sending Byte */
		case SENDING_BYTE :	switch(gu8_BCM_Index_Buffer_TX_Dispat)	
							{
								/* Send ID of BCM frame */
								case BCM_FIRST_BYTE : if (u8_UART_TX_Flag == BYTE_IS_SENT )		/* Check if previous Byte sent successfully */ 
												   {
													   UART_Send_Byte(str_BCM_Fram_TX.u8_BCM_ID);
													   u8_UART_TX_Flag = SENDING;
													   gu8_BCM_Index_Buffer_TX_Dispat++;
												   }
												   else
												   {
														/* Retrying  + Timeout handling */
												   }
		   
								break;
								/* Send Second Byte in BCM Frame  "Data Size" MOST  */
								case BCM_SECOND_BYTE :
												   if (u8_UART_TX_Flag == BYTE_IS_SENT )		/* Check if previous Byte sent successfully */ 
												   {
													   UART_Send_Byte((uint16_t)str_BCM_Fram_TX.u16_Data_Size >> 8);
													   u8_UART_TX_Flag = SENDING;
													   gu8_BCM_Index_Buffer_TX_Dispat++;
												   }	 
												   else
												   {
														/* Retrying  + Timeout handling */
												   }  
								break;
								/* Second Byte in BCM Frame  "Data Size" LEAST */
								case BCM_THIRD_BYTE :
												   if (u8_UART_TX_Flag == BYTE_IS_SENT )		/* Check if previous Byte sent successfully */ 
												   {
													    UART_Send_Byte((uint8_t)str_BCM_Fram_TX.u16_Data_Size);
														u8_UART_TX_Flag = SENDING;
														gu8_BCM_Index_Buffer_TX_Dispat++;
												   }
												   else
												   {
														/* Retrying  + Timeout handling */
												   }
								break;
							    /* Normal Case send the Data itself */
							    default:		   if (gu8_BCM_Index_Buffer_TX_Dispat < LAST_BYTE_TX)
												   {
														if (u8_UART_TX_Flag == BYTE_IS_SENT )	/* Check if previous Byte sent successfully */ 
														{
															str_BCM_Fram_TX.u8_Check_Sum+=str_BCM_Fram_TX.pu8_Data_Address[gu8_BCM_Index_Buffer_TX_Dispat-3];
															UART_Send_Byte(str_BCM_Fram_TX.pu8_Data_Address[gu8_BCM_Index_Buffer_TX_Dispat-3]);
															u8_UART_TX_Flag = SENDING;
															gu8_BCM_Index_Buffer_TX_Dispat++;
														}
														else
														{
															/* Retrying  + Timeout handling */
														}
												   }
												   else
												   {
														gu8_BCM_State_Machine_TX = SENDING_BYTE_COMPLETE ;
												   }
								break;
							}
		break; 
		/*  State 2 Sending Byte complete */
		case SENDING_BYTE_COMPLETE :	if( gu8_BCM_Index_Buffer_TX_Dispat == LAST_BYTE_TX)  /* check for the last byte to send */
										{
										
											gu8_BCM_State_Machine_TX = SENDING_FRAME_COMPLETE;
										}
										else
										{
											gu8_BCM_State_Machine_TX = SENDING_BYTE;  /* Change state from sending byte complete to sending byte */
											
										}			
		break;
		/*  State 3 Sending Frame Complete */
		case SENDING_FRAME_COMPLETE :   
										if (u8_UART_TX_Flag == BYTE_IS_SENT )		/* Check if previous Byte sent successfully */
										{
											UART_Send_Byte(str_BCM_Fram_TX.u8_Check_Sum);    	/* Send checksum */
											u8_UART_TX_Flag = SENDING;
											gu8_BCM_State_Machine_TX = IDLE ;			        /* Change state from sending frame complete to idle */
											gu8_lock_TX_Buffer = UNLOCKED ;						/* Unlock BCM able to get new requests */
											if(g_callBack_tx != NULL_PTR)					    /* call back APP function */
											{
												g_callBack_tx();
											}
											else
											{
												retval = ERROR_NULL_POINTER;	/* Error access null pointer */
											}
										}
										else
										{
											/* Retrying  + Timeout handling */
										}
		break;
		default:		retval = ERROR_WRONG_STATE;			/* Error wrong	state */
		break;
	}
	return retval;	
 }	

 /* BCM Setup RX Buffer to construct the buffer of receiving data */
 extern uint8_t BCM_SetupRX_Buffer(uint16_t u16_Data_Size ,uint8_t* pu8_Data_Address)
 {
	uint8_t retval = ERROR_OK;
	/* Check if RX BCM initialized or not */
	if ( gu8_BCM_init == ERROR_OK )
	{	
		/* check for Rx buffer locked or unlocked */
		if( gu8_lock_RX_Buffer == UNLOCKED )
		{	
			/* check for Null pointer */
			if (pu8_Data_Address != NULL_PTR)
			{
				str_BCM_Fram_RX.u8_BCM_ID = BCM_ID;				       /* assign the ID of BCM frame */
				str_BCM_Fram_RX.u16_Data_Size = u16_Data_Size;         /* assign data size to BCM frame */
				str_BCM_Fram_RX.pu8_Data_Address = pu8_Data_Address;   /* assign pointer of data BCM frame */
				str_BCM_Fram_RX.u8_Check_Sum = 0;					   /* Initial value of check sum equal zero */
				gu8_BCM_State_Machine_RX = IDLE ;                      /* Change state from idle to sending byte */
				gu8_setupRX_Buffer = SETUP_SUCCEEDED;				   /* indicate that RX buffer is successfully installed */
			}
			else
			{
				retval = ERROR_NULL_POINTER;					   /* Error access null pointer */
			}
		}
		else
		{
			retval = ERROR_BCM_LOCKED;							   /* BCM Locked , can't Receive unless it unlocked */
		}
	}
	else
	{
		retval = BCM_NOT_INITIALIZED;							   /* BCM must initialized first */
	}
	return retval;
 }

  /* BCM RX Dispatcher to switch between states and handle receiving data */
 extern uint8_t BCM_RxDispatcher(void)
 {
	uint8_t retval = ERROR_OK;
	switch (gu8_BCM_State_Machine_RX)
	{	/* State 0 Idle state */
		case IDLE :  /* DO Nothing you're in rest here */
		break;
		/*  State 1 receiving Byte */
		case RECEIVING_BYTE :     /* DO nothing , stay here ISR will move you to receiving byte complete */
		break;
		/*  State 1 receiving Byte complete */
		case RECEIVING_BYTE_COMPLETE : if(gu8_BCM_Index_Buffer_RX_Dispat == BCM_FIRST_BYTE)  /* receiving ID of BCM */
									   {
											if(str_BCM_Fram_RX.pu8_Data_Address[gu8_BCM_Index_Buffer_RX_Dispat] == BCM_ID )
											{
												/* Status OK */ /* ID Matched */
												gu8_BCM_Index_Buffer_RX_Dispat++;			/* increment flag RX Dispatcher */
												gu8_BCM_State_Machine_RX = RECEIVING_BYTE;  /* change state to receiving byte */
												
											}
											else
											{
												gu8_setupRX_Buffer = SETUP_FAILED;  /* Wrong BCM ID ... close BCM not interested in that data */
											}
											
									   }
									   else if (gu8_BCM_Index_Buffer_RX_Dispat == BCM_SECOND_BYTE)	/* receiving Sizer of BCM Most Byte */
									   {

											if (str_BCM_Fram_RX.pu8_Data_Address[gu8_BCM_Index_Buffer_RX_Dispat] ==  (uint16_t) (str_BCM_Fram_RX.u16_Data_Size>> 8))
											{	
												gu8_BCM_Index_Buffer_RX_Dispat++;			/* increment flag RX Dispatcher */
												gu8_BCM_State_Machine_RX = RECEIVING_BYTE;  /* change state to receiving byte */
											}
											else
											{
												gu8_setupRX_Buffer = SETUP_FAILED;  /* Wrong BCM ID ... close BCM not interested in that data */
											}
									   }
									   else if (gu8_BCM_Index_Buffer_RX_Dispat == BCM_THIRD_BYTE)    /* receiving Sizer of BCM Least Byte */
									   {
									   
											if (str_BCM_Fram_RX.pu8_Data_Address[gu8_BCM_Index_Buffer_RX_Dispat] ==  (uint8_t) str_BCM_Fram_RX.u16_Data_Size)
											{
												 /* Data Size Matched */
												gu8_BCM_Index_Buffer_RX_Dispat++;			/* increment flag RX Dispatcher */
												gu8_BCM_State_Machine_RX = RECEIVING_BYTE;  /* change state to receiving byte */
											}
											else
											{
												gu8_setupRX_Buffer = SETUP_FAILED;  /* Wrong BCM ID ... close BCM not interested in that data */
											}
											
									   }
									   else if (gu8_BCM_Index_Buffer_RX_Dispat < LAST_BYTE_RX )		/* case receiving data itself */
									   {
											str_BCM_Fram_RX.u8_Check_Sum +=  str_BCM_Fram_RX.pu8_Data_Address[gu8_BCM_Index_Buffer_RX_Dispat];    /* check sum calculate */
											gu8_BCM_Index_Buffer_RX_Dispat++;			/* increment flag RX Dispatcher */
											gu8_BCM_State_Machine_RX = RECEIVING_BYTE;	/* change state to receiving byte */
											
									   }
									   else			/* Here we received All frame according to size  */
									   {			/* NOW receive last byte ( Checksum ) */
									   
											if(str_BCM_Fram_RX.u8_Check_Sum == str_BCM_Fram_RX.pu8_Data_Address[gu8_BCM_Index_Buffer_RX_Dispat] )
											{
											
												/* check sum is equaled congratulations DATA is successfully sent */
												gu8_BCM_State_Machine_RX = RECEIVING_FRAME_COMPLETE ;/* change state to receiving frame */
											}
											else
											{
												/* Check sum is Wrong so BCM will got to SETUP_FAILED */
												gu8_setupRX_Buffer = SETUP_FAILED;
											}
									   }

		break;
		/*  State 3 Sending Frame Complete */
		case RECEIVING_FRAME_COMPLETE : /* frame is received successfully */
										gu8_lock_RX_Buffer = LOCKED ;		  /* Lock BCM to prevent new requests */
										gu8_BCM_State_Machine_RX = IDLE ;     /* Change state to IDLE */
										if(g_callBack_rx != NULL_PTR)		  /* call back APP function */
										{
											g_callBack_rx();				  /* Call APP customer */
										}
										else
										{
											retval = ERROR_NULL_POINTER;	 /* Error access null pointer */
										}								
		break;		

		default:		retval = ERROR_WRONG_STATE;			/* Error wrong	state */
		break;

	}
	return retval;
 }



 /* Callback function run when ISR of UART TX fire */
 extern void g_BCM_TX_callback(void)
 {	
	 u8_UART_TX_Flag = BYTE_IS_SENT;				     /* to indicate that byte already sent and ready to sent new one */
	 gu8_BCM_State_Machine_TX = SENDING_BYTE_COMPLETE ;  /* change state of TX to sending byte complete */
 }


 /* Callback function run when ISR of UART RX fire */
 extern void g_BCM_RX_callback(void)
 {
	/* Check if RX buffer is created or not */
	if(	gu8_setupRX_Buffer == SETUP_SUCCEEDED )			
	{	
		gu8_BCM_RX_data = 0;
		UART_Start_RecieveByte(&gu8_BCM_RX_data);								          /* ISR of UART RX so we receive data */
		str_BCM_Fram_RX.pu8_Data_Address[gu8_BCM_RX_Index_Buffer_ISR] = gu8_BCM_RX_data;  /* Fill the data in BCM RX Buffer */	

		if((str_BCM_Fram_RX.pu8_Data_Address[BCM_FIRST_BYTE] != BCM_ID ))	                  /* Check for ID of BCM */
		{
			gu8_setupRX_Buffer = SETUP_FAILED;        /* GET out because ID of BCM not Matched 
													     Here we not interested in data, every time ISR called
													     we won't save it or receive it */
		}
		else
		{
			/* You are Good here ID is correct - continue your work */
			gu8_BCM_RX_Index_Buffer_ISR++;	
			gu8_BCM_State_Machine_RX = RECEIVING_BYTE_COMPLETE;				/* Change status to Receiving byte complete */
		}
	}
	else
	{
		/* can't receive data while RX buffer not setup */
		/* we can here run function with flag to indicate Error */
	}

 }


 /* Global Callback Function to set by customer in APP to run after BCM Finish  */
 void BCM_setCallBack_TX(void(*APP)(void))
 {
	 g_callBack_tx = APP;
 }

/* Global Callback Function to set by customer in APP to run after BCM Finish  */
void BCM_setCallBack_RX(void(*APP)(void))
{
	g_callBack_rx = APP;
}

