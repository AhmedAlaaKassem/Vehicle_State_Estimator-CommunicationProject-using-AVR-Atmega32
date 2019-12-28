/*
 * DIO_PROJECT_TESTED.c
 *
 * Created: 12/17/2019 4:33:13 AM
 * Author : Ahmed Alaa
 */ 


 
 #include "DIO.h"
 #include "UART_Pb.h"
  #include "SPI_Pb.h"
 #include "APP.h"



 int main ()
 {
	
	 DIO_Init_Port(PORT_A,OUTPUTS);
	 DIO_Init_Port(PORT_C,OUTPUTS);
	 DIO_Init_Pin(C4,IN);
	 DIO_Init_Pin(B2,IN);
	 DIO_Init_Pin(B0,IN);
	 UART_Init(&obj_UART);						/* Init UART */
	 SPI_Init(&SPI_TX);							/* Init SPI */
	 Set_Call_Back_UART_TX(g_VSE_TX_callback);	 /* set the function that will run when UART TX ISR fire */
	 Set_Call_Back_UART_RX(g_VSE_RX_callback);   /* set the function that will run when UART RX ISR fire */
	 
	 while (1)
	 {
		VSE_dispatcher_KIT1();			       	/* Looping for states */
	 }
 }






