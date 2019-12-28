/*
 * APP.h
 *
 * Created: 12/18/2019 12:42:48 PM
 *  Author: ahmed
 */ 


#ifndef APP_H_
#define APP_H_


#include "DIO.h"

 /* Callback function run when ISR of UART TX fire */
 extern void g_VSE_TX_callback(void);

 /* Callback function run when ISR of UART RX fire */
 extern void g_VSE_RX_callback(void);

 extern void VSE_dispatcher_KIT1(void);



#endif /* APP_H_ */