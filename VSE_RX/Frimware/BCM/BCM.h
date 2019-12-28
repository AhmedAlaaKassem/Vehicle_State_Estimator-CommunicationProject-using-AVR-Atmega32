/*
 * BCM.h
 *
 * Created: 12/23/2019 11:19:39 PM
 *  Author: ahmed
 */ 

#ifndef BCM_H_
#define BCM_H_

#include "STD_TYPES.h"
#include "COMMON_MACROS.h"

/* BCM ID Constant */
#define BCM_ID 3    
  
/* Protocol ID */
#define UART 1
#define SPI 2

/* ERROR Handling */
#define ERROR_OK                        0	/* OK No Error */
#define BCM_NOT_INITIALIZED            -1	/* BCM Not initialized */
#define ERROR_UNDEFINED_COMM_PROTOCOL  -2	/* BCM didn't use UART or SPI */
#define ERROR_BCM_LOCKED               -4   /* BCM Locked , can't request unless unlocked */
#define ERROR_NULL_POINTER             -5   /* Error access null pointer */
#define ERROR_WRONG_STATE              -6	/* Error wrong	state */
#define ERROR_UART_NOT_INITIALIZED     -7	/* Error in UART Init */
#define SETUP_FAILED                   -8   /* Error faild to setup RX Buffer */


/* Initialization for BCM module */
extern uint8_t BCM_Init(uint8_t u8_Comm_Protocol);

/* BCM send change state to sending byte - start transmission */
extern uint8_t BCM_Send(uint16_t u16_Data_Size ,uint8_t* pu8_Data_Address);

/* BCM TX Dispatcher to switch between states and handle sending data */
extern uint8_t BCM_TxDispatcher(void);

/* BCM Setup RX Buffer to construct the buffer of receiving data */
extern uint8_t BCM_SetupRX_Buffer(uint16_t u16_Data_Size ,uint8_t* pu8_Data_Address);

/* BCM RX Dispatcher to switch between states and handle receiving data */
extern uint8_t BCM_RxDispatcher(void);

/* Callback function run when ISR of UART TX fire */
extern void g_BCM_TX_callback(void);

/* Callback function run when ISR of UART RX fire */
extern void g_BCM_RX_callback(void);

/* Global Callback Function to set by customer in APP to run after BCM Finish #TRANSIMITTING#  */
extern void BCM_setCallBack_TX(void(*APP)(void));

/* Global Callback Function to set by customer in APP to run after BCM Finish #RECEIVING#  */
extern void BCM_setCallBack_RX(void(*APP)(void));

#endif /* BCM_H_ */