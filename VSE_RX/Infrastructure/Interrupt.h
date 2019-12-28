/*
 * Interrupt.h
 *
 * Created: 16/12/2019 1:17:22 AM
 *  Author: Wazeer
 */ 


#ifndef __INTERRUPT_H__
#define __INTERRUPT_H__

#include "STD_TYPES.h"

/*
* Definitions bits of "SREG" Register that contains Status Register.
*/
#define SREG     *((volatile uint8_t*) 0x5FU)

#define GI 		7U

#define FLAG_IS_SET 		1U
#define FLAG_IS_CLEAR 		0U

/***************************************************************************************/
/* brief!  function that enable the global interrupt                                   */
/* param!  void                            			                                   */
/* return! void                                                                        */
/***************************************************************************************/
extern void Enable_GlobalInterrupt(void);

/***************************************************************************************/
/* brief!  function that disable the global interrupt                                  */
/* param!  void                            			                                   */
/* return! void                                                                        */
/***************************************************************************************/
extern void Disable_GlobalInterrupt(void);

#endif /* __INTERRUPT_H__ */
