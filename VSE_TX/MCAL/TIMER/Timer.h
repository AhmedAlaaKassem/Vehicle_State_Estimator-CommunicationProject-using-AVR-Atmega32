#ifndef TIMER_H_
#define TIMER_H_

#include "STD_TYPES.h"
#include "COMMON_MACROS.h"
#include "TIMER_REG.h"

typedef struct
{
  uint8_t u8_Channel;			/*Timer Channel (Timer0 or Timer1 or Timer2*/
  uint8_t u8_Psc;			    /*Timer Prescaler Based on DataSheet No prescaler or /8 or /64 or .....*/
  uint8_t u8_Timer_Mode;        /*Timer Mode Timer or Counter*/
  uint8_t u8_Interrupt_Mode;    /*Timer Overflow Mode Interrupt or polling*/
}Str_TimerConfiguration_t;

/*__________________________ MACROS ______________________________*/
#define NO_OF_CHANNELS 3
/*__TIMER_CHANNELS__*/
#define TIMER0 0
#define TIMER1 1
#define TIMER2 2
/*                        TIMER_CHANNELS OverFlow                 */
#define TIMER0_OVERFLOW 255
#define TIMER1_OVERFLOW 65536
#define TIMER2_OVERFLOW 255
/*                        TIMER_MODE                              */
#define TIMER_MODE 0
#define COUNTER_MODE 1

#define RESET 0
/*                        PreScaler                               */
/*________Timer0__________________________________*/
#define NO_CLOCK_TIMER_0     					0
#define F_CPU_CLOCK_TIMER_0   					1
#define F_CPU_CLOCK_8_TIMER_0 					2
#define F_CPU_CLOCK_64_TIMER_0					3
#define F_CPU_CLOCK_256_TIMER_0					4
#define F_CPU_CLOCK_1024_TIMER_0				5
#define F_EXTERNAL_CLOCK_FALLING_TIMER_0		6
#define F_EXTERNAL_CLOCK_RISING_TIMER_0			7
/*________Timer1__________________________________*/
#define NO_CLOCK_TIMER_1     					0
#define F_CPU_CLOCK_TIMER_1   					1
#define F_CPU_CLOCK_8_TIMER_1 					2
#define F_CPU_CLOCK_64_TIMER_1					3
#define F_CPU_CLOCK_256_TIMER_1					4
#define F_CPU_CLOCK_1024_TIMER_1				5
#define F_EXTERNAL_CLOCK_FALLING_TIMER_1		6
#define F_EXTERNAL_CLOCK_RISING_TIMER_1			7
/*________Timer2__________________________________*/
#define NO_CLOCK_TIMER_2     					0
#define F_CPU_CLOCK_TIMER_2   					1
#define F_CPU_CLOCK_8_TIMER_2 					2
#define F_CPU_CLOCK_32_TIMER_2					3
#define F_CPU_CLOCK_64_TIMER_2					4
#define F_CPU_CLOCK_128_TIMER_2					5
#define F_CPU_CLOCK_256_TIMER_2		 			6
#define F_CPU_CLOCK_1024_TIMER_2				7
/*                        OverFlow Mode                           */
#define POLLING      0
#define INTERRUPT    1
/*                        ERROR STATUS                            */
#define ERROR_OK     0
#define ERROR_NOK    1

/*                        FUNCTIONS PROTOTYPES                    */

/*Description: Timer/Counter Initialization
 * Input     : Timer_Configuration_S* Confg_S (Struct contain : Timer Channel, Prescaler, Timer mode , Mode as described in Struct)
 * Output    : Error Checking */
extern uint8_t Timer_Init(Str_TimerConfiguration_t* pstr_Timer_Confg);

/*Description: Timer/Counter Start
 * Input     :  Timer Channel(Timer Channel (Timer0 or Timer1 or Timer2), Tick Counting (Counts given by user)
 * Output    : Error Checking*/
extern uint8_t Timer_Start(uint8_t u8_TimerChannel,uint32_t u8_TickCounts);

/*Description: Timer/Counter Stop
 * Input     :  Timer Channel(Timer Channel (Timer0 or Timer1 or Timer2)
 * Output    : Error Checking */
extern uint8_t Timer_Stop(uint8_t u8_TimerChannel);

/*Description: Timer Reset
 * Input     :  Timer Channel(Timer Channel (Timer0 or Timer1 or Timer2)
 * Output    : Error Checking */
extern uint8_t Timer_Reset(uint8_t u8_TimerChannel);



#endif /* TIMER_H_ */
