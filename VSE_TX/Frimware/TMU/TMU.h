/*
 * TMU.h
 *
 * Created: 12/18/2019 9:03:23 AM
 *  Author: ahmed
 */ 


#ifndef TMU_H_
#define TMU_H_

#include "Timer.h"
#include "Timer_Pb.h"

/* _____TMU STRUCTURE____ */ 
typedef struct
{
	void(*pf_Function)(void);  
	uint16_t u16_TimeDelay;     
	uint8_t  u8_RepeatFunction;   
	uint8_t  u8_Ready;			 
}Str_TMU_Cfg;

/*_____ ERROR STATUS_____ */
#define ERROR_OK		 	           0
#define ERROR_NOK			           1
#define ERROR_RESOLUTION              -1  /* Resolution passed to TMU is too Large to handle */
#define ERROR_BUFFER_FULL             -2  /* Array Buffer of TMU is FULL */
#define ERROR_ACCESS_NULL_PTR         -3  /* Access Null Pointer Error */
#define ERROR_BUFFER_EMPTY            -4  /* Array Buffer of TMU is Empty */
#define ERROR_FUNCTION_STOP_NOT_FOUND -5  /* Can't find the function to stop it */

/*_________ MACROS________ */
#define TMU_PERIODIC 0
#define TMU_ONE_SHOT 1

#define FUNCTION_READY 0
#define FUNCTION_NOT_READY 1

#define TMU_BUFFER_SIZE   4
#define TIMER0_MAX_RES 31

#define TMU_INITIALIZED 0
#define TMU_NOT_INITIALIZED -6



/* FUNCTIONS PROTOTYBE */
extern uint8_t TMU_Init(uint16_t u16_Resolution);
extern uint8_t TMU_Start(Str_TMU_Cfg * pstr_TMU_Cfg);
extern uint8_t TMU_Stop(void(*pf_Function)(void));
extern uint8_t TMU_Dispatcher(void);
//extern uint8_t TMU_Stop();



#endif /* TMU_H_ */