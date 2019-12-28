 #include "TMU.h"
 #include "DIO.h"

 /* VARIABLES */
 static uint16_t gu16_Resolution;
 static uint16_t gu16_TickTime;
 static uint8_t gu8_BufferCount = 0;
 static uint8_t gu8_TMU_flag =0;
 static uint8_t gu8_TMU_init = TMU_NOT_INITIALIZED;
 static Str_TMU_Cfg gastr_JopBuffer[TMU_BUFFER_SIZE];
 static uint16_t ga_Counter[TMU_BUFFER_SIZE];

 /* ISR FOR TIMERS */
 void __vector_9(void)__attribute((signal,used));   /* TIMER1 OverFlow */
 void __vector_10(void)__attribute((signal,used));  /* TIMER0 Compare Match */
 void __vector_4(void)__attribute((signal,used));   /* TIMER2 Compare Match */

 /* Functions Implementations */
 extern uint8_t TMU_Init(uint16_t u16_Resolution)
 {
	 /* USEING ONLY TIMER0 .... in next versions handle more than 1 timer */
	 uint8_t retval = ERROR_OK;
	 if(u16_Resolution < TIMER0_MAX_RES)
	 {
		 Timer_Init(&Timer0);				/* Init Timer0 from Timer_Pb.c post_build file */
		 gu16_Resolution = u16_Resolution;	/* store Resolution in Global variable */
		 gu8_TMU_init = TMU_INITIALIZED ;			/* Global to check if TMU get initialized or not */
	 }
	 else
	 {
		 retval = ERROR_RESOLUTION;			/* Resolution passed to TMU is too Large to handle */
	 }
	 return retval;							/* Return Result of Execution */
 }

 extern uint8_t TMU_Start(Str_TMU_Cfg * pstr_TMU_Cfg)
 {
	 /* USEING ONLY TIMER0 .... in next versions handle more than 1 timer */
	 uint8_t retval = ERROR_OK;
	 /* Check if TMU get Initialized or not */
	 if ( gu8_TMU_init == TMU_INITIALIZED )
	 {	/* Check for NULL pointer */
		 if( pstr_TMU_Cfg != NULL_PTR)
		 {	/* Check for NULL pointer */
			 if( pstr_TMU_Cfg -> pf_Function != NULL_PTR )
			 {
				 /* Fill the Global Array with the input TMU passed Structure */
				 gastr_JopBuffer[gu8_BufferCount].pf_Function       = pstr_TMU_Cfg -> pf_Function;
				 gastr_JopBuffer[gu8_BufferCount].u16_TimeDelay     = pstr_TMU_Cfg -> u16_TimeDelay;
				 gastr_JopBuffer[gu8_BufferCount].u8_RepeatFunction = pstr_TMU_Cfg -> u8_RepeatFunction;
				 gastr_JopBuffer[gu8_BufferCount].u8_Ready          = pstr_TMU_Cfg -> u8_Ready;
				 ga_Counter[gu8_BufferCount]=0;
				 gu8_BufferCount++;					/* increment Global counter of Buffer */
				 if(gu8_BufferCount <= TMU_BUFFER_SIZE )
				 {
					 /* check for entrance once to ensure TMU Start one Time */
					 if(gu8_TMU_flag == 0)
					 {
						 gu8_TMU_flag = 1;				  /* Change Global flag */
						 gu16_TickTime = gu16_Resolution * (1000UL / 128UL) ;    /* Calculate the Tick_Time */
						 Timer_Start(TIMER0,gu16_TickTime);						/* Start Timer0 */
					 }
				 }
				 else
				 {
					 retval = ERROR_BUFFER_FULL;								/* Array Buffer of TMU is FULL */
				 }
			 }
			 else
			 {
				 retval = ERROR_ACCESS_NULL_PTR;								/* Access Null Pointer Error */
			 }
		 }
		 else
		 {
			 retval = ERROR_ACCESS_NULL_PTR;								/* Access Null Pointer Error */
		 }
	 }
	 else
	 {
		 retval = TMU_NOT_INITIALIZED ;								/* TMU Not initialized */
	 }
	 return retval;													/* Return Result of Execution */
 }


 extern uint8_t TMU_Dispatcher(void)
 {
	 /* USEING ONLY TIMER0 .... in next versions handle more than 1 timer */
	 uint8_t retval = ERROR_OK;
	 uint8_t volatile u8_count=0;
	 if( gu8_BufferCount > 0)
	 {	/* Check for Buffer counter */
		 for(u8_count; u8_count < gu8_BufferCount ; u8_count++ )
		 {
			 /* check of Time Delay meets the counter */
			 if( gastr_JopBuffer[u8_count].u16_TimeDelay == ga_Counter[u8_count] )
			 {
				 /* Check if the function is Ready for Execution */
				 if( gastr_JopBuffer[u8_count].u8_Ready == FUNCTION_READY )
				 {
					 ga_Counter[u8_count] = 0;					/* Set counter to 0 for Periodic executions */
					 gastr_JopBuffer[u8_count].pf_Function();	/* Execute the Function */
					 if( gastr_JopBuffer[u8_count].u8_RepeatFunction == TMU_ONE_SHOT )		/* Check if one shot */
					 {
						 gastr_JopBuffer[u8_count].u8_Ready = FUNCTION_NOT_READY ;		/* make it not ready to stop execution */
					 }
				 }
			 }
		 }
	 }
	 else
	 {
		 retval = ERROR_BUFFER_EMPTY;					/* Array Buffer of TMU is Empty */
	 }
	 return retval;										/* Return Result of Execution */
 }

 extern uint8_t TMU_Stop(void(*pf_Function)(void))
 {
	 uint8_t retval = ERROR_OK;
	 uint8_t volatile u8_count =0;
	 if ( pf_Function != NULL_PTR )
	 {
		 /* Looping into Buffer to search for function to delete */
		 for( u8_count;u8_count< gu8_BufferCount ;u8_count++ )
		 {
			 if(gastr_JopBuffer[u8_count].pf_Function == pf_Function  )
			 {
				 gastr_JopBuffer[u8_count].u8_Ready = FUNCTION_NOT_READY;  /* Stop function */
			 }
			 else
			 {
				 retval = ERROR_FUNCTION_STOP_NOT_FOUND;		/* Can't find the function to stop it */
			 }
		 }

	 }
	 else
	 {
		 retval = ERROR_ACCESS_NULL_PTR;						/* Access Null Pointer Error */
	 }
	 return retval;
 }

 /*___Timer0 COMP Interrupt___*/
 void __vector_10 (void)
 {
	 uint8_t volatile u8_count =0;
	 for( u8_count;u8_count< gu8_BufferCount ;u8_count++ )
	 {
		 /* increase counter from every ISR */
		 ga_Counter[u8_count]++;
	 }
 }