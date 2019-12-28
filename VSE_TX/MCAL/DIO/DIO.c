#include "DIO.h"

uint8_t  DIO_Init_Pin  (uint8_t u8_Pin_Number ,uint8_t u8_Pin_State)
{
	uint8_t retval = ERROR_OK;
	
	/* Check Bit in PORT A */
	if(( u8_Pin_Number >= A0 ) && ( u8_Pin_Number <=A7 ))
	{
		if ( u8_Pin_State == OUT )
		{
			SET_BIT (DDRA,u8_Pin_Number);
		}
			else if ( u8_Pin_State == IN  )
			{
				CLR_BIT(DDRA,u8_Pin_Number);
			}
				else
				{
					retval = ERROR_NOK;
				}
	}
	/* Check Bit in PORT B */
	else if(( u8_Pin_Number >= B0 ) && ( u8_Pin_Number <=B7 ))
	{
		u8_Pin_Number -= AVR_PORT_BITS;
	
		if ( u8_Pin_State == OUT )
		{
			SET_BIT(DDRB,u8_Pin_Number);
		}
			else if ( u8_Pin_State == IN  )
			{
				CLR_BIT(DDRB,u8_Pin_Number);
			}
				else
				{
					retval = ERROR_NOK;
				}
	}
	/* Check Bit in PORT C */
	else if(( u8_Pin_Number >= C0 ) && ( u8_Pin_Number <=C7 ))
	{
		u8_Pin_Number -= AVR_PORT_BITS;
		u8_Pin_Number -= AVR_PORT_BITS;

		if ( u8_Pin_State == OUT )
		{
			SET_BIT(DDRC,u8_Pin_Number);
		}
			else if ( u8_Pin_State == IN  )
			{
				CLR_BIT(DDRC,u8_Pin_Number);
			}
				else
				{
					retval = ERROR_NOK;
				}
	}
	/* Check Bit in PORT D */
	else if(( u8_Pin_Number >= D0 ) && ( u8_Pin_Number <=D7 ))
	{
		u8_Pin_Number -= AVR_PORT_BITS;
		u8_Pin_Number -= AVR_PORT_BITS;
		u8_Pin_Number -= AVR_PORT_BITS;

		if ( u8_Pin_State == OUT )
		{
			SET_BIT(DDRD,u8_Pin_Number);
		}
			else if ( u8_Pin_State == IN  )
			{
				CLR_BIT(DDRD,u8_Pin_Number);
			}
				else
				{
					retval = ERROR_NOK;
				}
	}
	/* ERROR Bit Number is wrong */
	else
	{
		retval = ERROR_NOK;
	}
	return retval;	
}


uint8_t  DIO_Init_Port (uint8_t u8_Port_Number,uint8_t u8_Port_State)
{
	uint8_t retval = ERROR_OK;
	
	/* Check for PORT A */
	if( u8_Port_Number == PORT_A )
	{
		if( u8_Port_State == OUTPUTS )
		{
			DDRA = OUTPUTS;
		}
			else if( u8_Port_State == INPUTS )
			{
				DDRA = INPUTS;
			}
				else 
				{
					retval = ERROR_NOK;
				}
	}
	/* Check for PORT B */
	else if( u8_Port_Number == PORT_B )
	{
		if( u8_Port_State == OUTPUTS )
		{
			DDRB = OUTPUTS;
		}
			else if( u8_Port_State == INPUTS )
			{
				DDRB = INPUTS;
			}
				else 
				{
					retval = ERROR_NOK;
				}
	}
	/* Check for PORT C */
	else if( u8_Port_Number == PORT_C )
	{
		if( u8_Port_State == OUTPUTS )
		{
			DDRC = OUTPUTS;
		}
			else if( u8_Port_State == INPUTS )
			{
				DDRC = INPUTS;
			}
				else 
				{
				retval = ERROR_NOK;
				}
	}
	/* Check for PORT D */
	else if( u8_Port_Number == PORT_D )
	{
		if( u8_Port_State == OUTPUTS )
		{
			DDRD = OUTPUTS;
		}
			else if( u8_Port_State == INPUTS )
			{
				DDRD = INPUTS;
			}
				else 
				{
					retval = ERROR_NOK;
				}
	}
	/* Error PORT input */
	else
	{
		retval = ERROR_NOK;
	}
	return retval;
}


uint8_t  DIO_Write_Pin (uint8_t u8_Pin_Number,uint8_t Pin_Value)
{
	uint8_t retval = ERROR_OK;
	/* Check Bit in PORT A */
	if(( u8_Pin_Number >= A0 ) && ( u8_Pin_Number <=A7 ))
	{
		if ( Pin_Value == HIGH )
		{
			SET_BIT (PORTA,u8_Pin_Number);
		}
			else if ( Pin_Value == LOW  )
			{
				CLR_BIT(PORTA,u8_Pin_Number);
			}
				else
				{
					retval = ERROR_NOK;
				}
	}
	/* Check Bit in PORT B */
	else if(( u8_Pin_Number >= B0 ) && ( u8_Pin_Number <=B7 ))
	{
		u8_Pin_Number -= AVR_PORT_BITS;
		if ( Pin_Value == HIGH )
		{
			SET_BIT(PORTB,u8_Pin_Number);
		}
			else if ( Pin_Value == LOW  )
			{
				CLR_BIT(PORTB,u8_Pin_Number);
			}
				else
				{
					retval = ERROR_NOK;
				}
	}
	/* Check Bit in PORT C */
	else if(( u8_Pin_Number >= C0 ) && ( u8_Pin_Number <=C7 ))
	{
		u8_Pin_Number -= AVR_PORT_BITS;
		u8_Pin_Number -= AVR_PORT_BITS;
		if ( Pin_Value == HIGH )
		{
			SET_BIT(PORTC,u8_Pin_Number);
		}
			else if ( Pin_Value == LOW  )
			{
				CLR_BIT(PORTC,u8_Pin_Number);
			}
				else
				{
					retval = ERROR_NOK;
				}
	}
	/* Check Bit in PORT D */
	else if(( u8_Pin_Number >= D0 ) && ( u8_Pin_Number <=D7 ))
	{
		u8_Pin_Number -= AVR_PORT_BITS;
		u8_Pin_Number -= AVR_PORT_BITS;
		u8_Pin_Number -= AVR_PORT_BITS;
		if ( Pin_Value == HIGH )
		{
			SET_BIT(PORTD,u8_Pin_Number);
		}
			else if ( Pin_Value == LOW  )
			{
				CLR_BIT(PORTD,u8_Pin_Number);
			}
				else
				{
					retval = ERROR_NOK;
				}
	}
	/* ERROR Bit Number is wrong */
	else
	{
		retval = ERROR_NOK;
	}
	return retval;

}


uint8_t  DIO_Read_Pin  (uint8_t u8_Pin_Number,uint8_t *pu8_Reading)
{
	uint8_t retval = ERROR_OK;
	/* Check the Reading pointer */
	if (pu8_Reading == NULL)
	{
		/* return NOK as the pointer passed is pointing to NULL */
		retval = ERROR_NOK;
	}
	/* Reading pointer is valed */
	else
	{
		/* Check Bit in PORT A */
		if(( u8_Pin_Number >= A0 ) && ( u8_Pin_Number <=A7 ))
		{
			*pu8_Reading = READ_BIT(PINA,u8_Pin_Number);
		}
			/* Check Bit in PORT B */
			else if(( u8_Pin_Number >= B0 ) && ( u8_Pin_Number <=B7 ))
			{
				u8_Pin_Number -= AVR_PORT_BITS;
				*pu8_Reading = READ_BIT(PINB,u8_Pin_Number);
			
			}
				/* Check Bit in PORT C */
				else if(( u8_Pin_Number >= C0 ) && ( u8_Pin_Number <=C7 ))
				{
					u8_Pin_Number -= AVR_PORT_BITS;
					u8_Pin_Number -= AVR_PORT_BITS;
					*pu8_Reading = READ_BIT(PINC,u8_Pin_Number);
					
				}
					/* Check Bit in PORT D */
					else if(( u8_Pin_Number >= D0 ) && ( u8_Pin_Number <=D7 ))
					{
						u8_Pin_Number -= AVR_PORT_BITS;
						u8_Pin_Number -= AVR_PORT_BITS;
						u8_Pin_Number -= AVR_PORT_BITS;
						*pu8_Reading = READ_BIT(PIND,u8_Pin_Number);
					}
						/* ERROR Bit Number is wrong */
						else
						{
							retval = ERROR_NOK;
						}
	}
	return retval;
	
	
	
}


uint8_t  DIO_Write_Port(uint8_t u8_Port_Number,uint8_t Port_Value)
{
	uint8_t retval = ERROR_OK;
	
	if( u8_Port_Number == PORT_A )
	{
		if( Port_Value == PORT_HIGH )
		{
			PORTA = PORT_HIGH;
		}
			else if( Port_Value == PORT_LOW )
			{
				PORTA = PORT_LOW;
			}
				else if ( Port_Value > PORT_LOW && Port_Value < PORT_HIGH )
				{
					PORTA = Port_Value;
				}
					else
					{
						retval = ERROR_NOK;
					}
	}
	
	else if( u8_Port_Number == PORT_B )
	{
		if( Port_Value == PORT_HIGH )
		{
			PORTB = PORT_HIGH;
		}
			else if( Port_Value == PORT_LOW )
			{
				PORTB = PORT_LOW;
			}
				else if ( Port_Value > PORT_LOW && Port_Value < PORT_HIGH )
				{
					PORTB = Port_Value;
				}
					else
					{
						retval = ERROR_NOK;
					}
	}
	
	else if( u8_Port_Number == PORT_C )
	{
		if( Port_Value == PORT_HIGH )
		{
			PORTC = PORT_HIGH;
		}
			else if( Port_Value == PORT_LOW )
			{
				PORTC = PORT_LOW;
			}
				else if ( Port_Value > PORT_LOW && Port_Value < PORT_HIGH )
				{
					PORTC = Port_Value;
				}
					else
					{
						retval = ERROR_NOK;
					}
	}
	
	else if( u8_Port_Number == PORT_D )
	{
		if( Port_Value == PORT_HIGH )
		{
			PORTD = PORT_HIGH;
		}
			else if( Port_Value == PORT_LOW )
			{
				PORTD = PORT_LOW;
			}
				else if ( Port_Value > PORT_LOW && Port_Value < PORT_HIGH )
				{
					PORTD = Port_Value;
				}
					else
					{
						retval = ERROR_NOK;
					}
	}
	else
	{
		retval = ERROR_NOK;
	}
	return retval;	
}


uint8_t  DIO_Read_Port (uint8_t u8_Port_Number,uint8_t *pu8_Reading)
{
	uint8_t retval = ERROR_OK;
	
	if (pu8_Reading == NULL)
	{
		retval = ERROR_NOK;
	}
	
	else
	{
		if( u8_Port_Number == PORT_A )
		{
			*pu8_Reading = PINA;
		}
		
			else if( u8_Port_Number == PORT_B )
			{
				*pu8_Reading = PINB;
			}
			
				else if( u8_Port_Number == PORT_C )
				{
					*pu8_Reading = PINC;
				}
				
					else if( u8_Port_Number == PORT_D )
					{
						*pu8_Reading = PIND;
					}
						else 
						{
							retval = ERROR_NOK;
						}
	}

	return retval;	
}







