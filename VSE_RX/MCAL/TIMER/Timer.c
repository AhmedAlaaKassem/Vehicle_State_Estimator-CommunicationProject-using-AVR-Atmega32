#include "Timer.h"
#include "TIMER_REG.h"
#include "DIO.h"

static uint8_t gau8_TimerPrescaller[NO_OF_CHANNELS] = {NULL,NULL,NULL};     /* Global Array to store prescaler */ 
static uint8_t gau8_TimerMode[NO_OF_CHANNELS] = {NULL,NULL,NULL};			/* Global Array to store Timer_Mode */ 
   
extern uint8_t Timer_Init(Str_TimerConfiguration_t* pstr_Timer_Confg)
{
	uint8_t retval = ERROR_OK;  
	/* Check for Timer Channel */
	switch(pstr_Timer_Confg->u8_Channel)               
	{	
		case TIMER0:   
			/* Check for Timer_Mode */                           
			switch (pstr_Timer_Confg -> u8_Timer_Mode)
			{
				case TIMER_MODE   : gau8_TimerPrescaller[TIMER0] = pstr_Timer_Confg -> u8_Psc;
				break;
				case COUNTER_MODE : gau8_TimerPrescaller[TIMER0] = pstr_Timer_Confg -> u8_Psc;
				break;
				default : retval = ERROR_NOK ;
				break;
			}
			/* Check for Interrupt_Mode */ 
			switch (pstr_Timer_Confg -> u8_Interrupt_Mode)
			{
				case POLLING	: gau8_TimerMode[TIMER0] = pstr_Timer_Confg -> u8_Interrupt_Mode;
				break;
				case INTERRUPT  : gau8_TimerMode[TIMER0] = pstr_Timer_Confg -> u8_Interrupt_Mode;
				break;
				default : retval = ERROR_NOK ;
				break;
			}
			/* Check for valued Prescaler */ 
			switch (pstr_Timer_Confg -> u8_Psc)
			{
				case NO_CLOCK_TIMER_0 : 
				break;
				case F_CPU_CLOCK_TIMER_0 :
				break;
				case F_CPU_CLOCK_8_TIMER_0 :
				break;
				case F_CPU_CLOCK_64_TIMER_0 :
				break;
				case F_CPU_CLOCK_256_TIMER_0 :
				break;
				case F_CPU_CLOCK_1024_TIMER_0 :
				break;
				default : retval = ERROR_NOK ;
				break;
			}
		break;
		/*  Case of TIMER1  */
		case TIMER1: 
			/* Check for Timer_Mode */  
			switch(pstr_Timer_Confg -> u8_Timer_Mode)         
			{
				case TIMER_MODE   : gau8_TimerPrescaller[TIMER1] = pstr_Timer_Confg ->u8_Psc; 
				break;
				case COUNTER_MODE : gau8_TimerPrescaller[TIMER1] = pstr_Timer_Confg ->u8_Psc; 
				break;
				default : retval = ERROR_NOK; 
				break;
			}
			/* Check for Interrupt_Mode */
			switch(pstr_Timer_Confg -> u8_Interrupt_Mode)           
			{
				case POLLING	: gau8_TimerMode[TIMER1] = pstr_Timer_Confg -> u8_Interrupt_Mode;
				break;
				case INTERRUPT  : gau8_TimerMode[TIMER1] = pstr_Timer_Confg -> u8_Interrupt_Mode;
				break;
				default : retval = ERROR_NOK ;
				break;
			}
			/* Check for valued Prescaler */ 
			switch (pstr_Timer_Confg -> u8_Psc)
			{
				case NO_CLOCK_TIMER_1 :
				break;
				case F_CPU_CLOCK_TIMER_1 :
				break;
				case F_CPU_CLOCK_8_TIMER_1 :
				break;
				case F_CPU_CLOCK_64_TIMER_1 :
				break;
				case F_CPU_CLOCK_256_TIMER_1 :
				break;
				case F_CPU_CLOCK_1024_TIMER_1 :
				break;
				default : retval = ERROR_NOK ;
				break;
			}
		break;
		/*  Case of TIMER2  */
		case TIMER2:
			/* Check for Timer_Mode */ 
			switch (pstr_Timer_Confg -> u8_Timer_Mode)
			{
				case TIMER_MODE   : gau8_TimerPrescaller[TIMER2] = pstr_Timer_Confg -> u8_Psc;
				break;
				case COUNTER_MODE : gau8_TimerPrescaller[TIMER2] = pstr_Timer_Confg -> u8_Psc;
				break;
				default : retval = ERROR_NOK ;
				break;
			}
			/* Check for Interrupt_Mode */
			switch (pstr_Timer_Confg -> u8_Interrupt_Mode)
			{
				case POLLING	: gau8_TimerMode[TIMER2] = pstr_Timer_Confg -> u8_Interrupt_Mode;
				break;
				case INTERRUPT  : gau8_TimerMode[TIMER2] = pstr_Timer_Confg -> u8_Interrupt_Mode;
				break;
				default : retval = ERROR_NOK ;
				break;
			}
			/* Check for valued Prescaler */ 
			switch (pstr_Timer_Confg -> u8_Psc)
			{
				case NO_CLOCK_TIMER_2 :
				break;
				case F_CPU_CLOCK_TIMER_2 :
				break;
				case F_CPU_CLOCK_8_TIMER_2 :
				break;
				case F_CPU_CLOCK_32_TIMER_2 :
				break;
				case F_CPU_CLOCK_64_TIMER_2 :
				break;
				case F_CPU_CLOCK_128_TIMER_2 :
				break;
				case F_CPU_CLOCK_256_TIMER_2 :
				break;
				case F_CPU_CLOCK_1024_TIMER_2 :
				break;
				default : retval = ERROR_NOK ;
				break;
			}
		break;
		default :  retval = ERROR_NOK;  
		break;
	}
	return retval;   
}


extern uint8_t Timer_Start(uint8_t u8_TimerChannel,uint32_t u8_TickCounts)
{
	uint8_t retval = ERROR_OK;
	switch(u8_TimerChannel)                                       
	{
		case TIMER0 :                                 
			if(u8_TickCounts < TIMER0_OVERFLOW)
			{
				switch(gau8_TimerMode[TIMER0])              
				{
					case POLLING :	SET_BIT(TIMER0_TCCR0,TIMER0_TCCR0_WGM01);
									CLR_BIT(TIMER0_TCCR0,TIMER0_TCCR0_WGM00); 
									CLR_BIT(TIMER0_TIMSK,TIMER0_TIMSK_OCIE0);                                        
									TIMER0_TCCR0 |= gau8_TimerPrescaller[TIMER0];  
									TIMER0_OCR0 = u8_TickCounts;
					break;
					case INTERRUPT:	SET_BIT(TIMER0_TCCR0,TIMER0_TCCR0_WGM01);
									CLR_BIT(TIMER0_TCCR0,TIMER0_TCCR0_WGM00);
									SET_BIT(SREG,GLOBAL_INTERRUPT_ENABLE);  
									SET_BIT(TIMER0_TIMSK,TIMER0_TIMSK_OCIE0);
									TIMER0_TCCR0 |= gau8_TimerPrescaller[TIMER0];  
									TIMER0_OCR0 = u8_TickCounts;
									//DIO_Write_Pin(9,1);
					break;
					default: retval = ERROR_NOK;
					break;
				}
			}
			else
			{
				retval = ERROR_NOK; 
			}
			break;


		case TIMER1 :                                 
		if(u8_TickCounts < TIMER1_OVERFLOW)
		{
			TIMER1_TCNT1 = TIMER1_OVERFLOW - u8_TickCounts;
			switch(gau8_TimerMode[TIMER1])                
			{
				case POLLING :  TIMER1_TCCR1B &= 0xF8;                     
								TIMER1_TCCR1B |= gau8_TimerPrescaller[TIMER1];  
								SET_BIT(TIMER1_TIFR,TIMER1_TIFR_TOV1);
								TIMER1_TCNT1 = 0;
								break;
				case INTERRUPT: TIMER1_TCCR1B &= 0xF8;                     
								TIMER1_TCCR1B |= gau8_TimerPrescaller[TIMER1];  
								SET_BIT(TIMER1_TIMSK,TIMER1_TIMSK_TOIE1);   
								SET_BIT(SREG,GLOBAL_INTERRUPT_ENABLE);  
								break;
				default: retval = ERROR_NOK;
						 break;
			}
		}
		else
		{
			retval = ERROR_NOK; 
		}
		break;



		case TIMER2:
			if(u8_TickCounts < TIMER2_OVERFLOW)
			{
				switch(gau8_TimerMode[TIMER2])
				{
					case POLLING :	SET_BIT(TIMER2_TCCR2,TIMER2_TCCR2_WGM21);
									CLR_BIT(TIMER2_TCCR2,TIMER2_TCCR2_WGM20);
									CLR_BIT(TIMER2_TIMSK,TIMER2_TIMSK_OCIE2);
									TIMER2_TCCR2 |= gau8_TimerPrescaller[TIMER2];
									TIMER2_OCR2 = u8_TickCounts;
					break;
					case INTERRUPT:	SET_BIT(TIMER2_TCCR2,TIMER2_TCCR2_WGM21);
									CLR_BIT(TIMER2_TCCR2,TIMER2_TCCR2_WGM20);
									SET_BIT(SREG,GLOBAL_INTERRUPT_ENABLE);
									SET_BIT(TIMER2_TIMSK,TIMER2_TIMSK_OCIE2);
									TIMER2_TCCR2 |= gau8_TimerPrescaller[TIMER2];
									TIMER2_OCR2 = u8_TickCounts;
					break;
					default: retval = ERROR_NOK;
					break;
				}
			}
			else
			{
				retval = ERROR_NOK;
			}
			break;
		default:  retval = ERROR_NOK; 
		break;
	}
	return retval; 
}


extern uint8_t Timer_Stop(uint8_t u8_TimerChannel)
{
	uint8_t retval = ERROR_OK; /*Error Status : No Error*/
	switch(u8_TimerChannel)
	{
		case TIMER0 :	TIMER0_TCCR0 &= 0xf8; 
						break;
		case TIMER1 :	TIMER1_TCCR1B &= 0xf8; 
						break;
		case TIMER2 :   TIMER2_TCCR2 &= 0xf8;
						break;
		default:		retval = ERROR_NOK; /*Error Status : Error*/
	}
	return retval; /*Return Error Status*/
}


extern uint8_t Timer_Reset(uint8_t u8_TimerChannel)
{
	uint8_t retval = ERROR_OK;  /*Error Status : No Error*/

	switch(u8_TimerChannel)
	{
		case TIMER0 :   TIMER0_TCCR0 = RESET; /*Reset Counter */
						break;
		case TIMER1 :	TIMER1_TCNT1 = RESET; /*Reset Counter */
						break;
		case TIMER2 :   TIMER2_TCCR2 = RESET; /*Reset Counter */
		break;
		default:
			retval = ERROR_NOK; /*Error Status : Error*/
	}
	return retval; /*Return Error Status*/
}


/* static void (*TIMER0_CallBack)(void)=0; */