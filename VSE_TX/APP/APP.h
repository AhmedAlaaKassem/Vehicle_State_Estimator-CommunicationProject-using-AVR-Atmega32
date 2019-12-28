/*
 * APP.h
 *
 * Created: 12/18/2019 12:42:48 PM
 *  Author: ahmed
 */ 


#ifndef APP_H_
#define APP_H_

#include "DIO.h"

/* Callback function run when ISR of SPI fire */
extern void g_VSE_TX_callbackSPI(void);

 /* Callback function run when ISR of UART TX fire */
 extern void g_VSE_TX_callback(void);

/* Dispatcher function for changing states */
extern void VSE_dispatcher_KIT2(void);


#endif /* APP_H_ */