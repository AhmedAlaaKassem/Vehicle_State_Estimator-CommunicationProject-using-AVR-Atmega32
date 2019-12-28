/*
 * Interrupt.c
 *
 * Created: 16/12/2019 1:17:22 AM
 *  Author: Wazeer
 */

#include "Interrupt.h"
#include "COMMON_MACROS.h"

/***************************************************************************************/
/* brief!  function that enable the global interrupt                                   */
/* param!  void                            			                                   */
/* return! void                                                                        */
/***************************************************************************************/
void Enable_GlobalInterrupt(void)
{
	/*
	*	Enable global interrupt
	*/
	SET_BIT(SREG, GI);
}

/***************************************************************************************/
/* brief!  function that disable the global interrupt                                  */
/* param!  void                            			                                   */
/* return! void                                                                        */
/***************************************************************************************/
void Disable_GlobalInterrupt(void)
{
	/*
	*	Disable global interrupt
	*/
	CLEAR_BIT(SREG, GI);
}
