/*
 * DIO_PROJECT_TESTED.c
 *
 * Created: 12/17/2019 4:33:13 AM
 * Author : Ahmed Alaa
 */ 
 
 #include "DIO.h"
 #include "SPI_Pb.h"
 #include "UART_Pb.h"
 #include "DIO.h"
 #include "APP.h"

 int main(void)
 {
	SPI_Init(&SPI_RX);							/* Init SPI */
	UART_Init(&obj_UART);						/* Init UART */
	Set_Call_Back_SPI(g_VSE_TX_callbackSPI);	/* Set call back function for SPI */
	Set_Call_Back_UART_TX(g_VSE_TX_callback);	/* set call back function for UART TX */

	DIO_Init_Pin(C4,IN);						/* Bush Button to fetch data */
	DIO_Init_Pin(B0,OUT);	 					/* feed back to kit 1 to fetch new speed an reset timer */

	 while (1)
	 {
		VSE_dispatcher_KIT2();					/* Looping for states */
	 }
 }


