/*
 SPI.c
 *
 * Created: 12/17/2019 4:33:13 AM
 * Author : Ahmed Alaa
 */ 


#include "SPI.h"
#include "DIO.h"

uint8_t gu8_Data_SentFlag = 1;		   /* Global variable used by APP to check for data sent */
									   /* This variable is Cleared by ISR of SPI ___________ */
uint8_t gu8_Interrupt_Polling_Mode=0;  /* Global variable to store if using Interrupt or Polling */

static void(*callBack_spi)(void)= NULL_PTR;


/* Description : SPI Initialization                                              */
/* Input       : Str_SPIConfiguration_t* "Pointer to Struct of SPI Configration" */
/* Output      : Error Checking                                                  */
uint8_t SPI_Init(Str_SPIConfiguration_t* pstr_SPI_Confg)
{
	uint8_t retval = ERROR_OK;
	/* Check for MASTER OR SLAVE  */
	if( pstr_SPI_Confg -> u8_Master_Mode == MASTER )
	{
		/* Set Maser Mode */
		SET_BIT(SPCR,MSTR);
		/* Set PINS of SPI */
		DIO_Init_Pin(B4,OUT);   /*   SS(PB4)   --> Output */
		DIO_Init_Pin(B5,OUT);   /*   MOSI(PB5) --> Output */
		DIO_Init_Pin(B6,IN);    /*   MISO(PB6) --> Input  */
		DIO_Init_Pin(B7,OUT);   /*   SCK(PB7) --> Output  */

		/* Check for Normal Speed or Double speed Mode */
		if (pstr_SPI_Confg ->u8_Speed_Mode == NORMAL_SPEED)
		{
			CLR_BIT(SPSR,SPI2X);
			/* Check for prescaler of SPI */
			switch( pstr_SPI_Confg -> u8_Pre_Scaler )
			{
				case PRE_SCALER_F_4   :  CLR_BIT(SPCR,SPR0);  /* set prescaler 0 , 0 */
										 CLR_BIT(SPCR,SPR1);					 
										 break;
				case PRE_SCALER_F_16  :  SET_BIT(SPCR,SPR0);  /* set prescaler 0 , 1 */
										 CLR_BIT(SPCR,SPR1);
										 break;
				case PRE_SCALER_F_64  :  CLR_BIT(SPCR,SPR0);  /* set prescaler 1 , 0 */
										 SET_BIT(SPCR,SPR1); 
										 break;
				case PRE_SCALER_F_128 :  SET_BIT(SPCR,SPR0);  /* set prescaler 1 , 1 */
										 SET_BIT(SPCR,SPR1);
										 break;
				default: retval = ERROR_PRE_SCALER;           /* Error on prescaler input */
								  break;
			}
		}
		else if (pstr_SPI_Confg -> u8_Speed_Mode == DOUBLE_SPEED )
		{
			/* Set Double Speed Mode */
			SET_BIT(SPSR,SPI2X);
			/* Check for prescaler of SPI */
			switch( pstr_SPI_Confg -> u8_Pre_Scaler )
			{
				case PRE_SCALER_F_2   : CLR_BIT(SPCR,SPR0);
										CLR_BIT(SPCR,SPR1);
										break;
				case PRE_SCALER_F_8   : SET_BIT(SPCR,SPR0);
										CLR_BIT(SPCR,SPR1);
										break;
				case PRE_SCALER_F_32  : CLR_BIT(SPCR,SPR0);
										SET_BIT(SPCR,SPR1);
										break;
				/* Last Case of F_64 not handled in this mode you can use it from Normal Mode */
				default: retval = ERROR_PRE_SCALER;
				break;
			}
		}
		else
		{
			retval = ERROR_SPI_SPEED_MODE ;     /* wrong SPI speed mode */
		}

		/*** Check if Interrupt or Polling ***/
		if(pstr_SPI_Confg -> u8_Interrupt_Mode == INTERRUPT) 
		{
			SET_BIT(SPCR,SPIE);                       /* Enable Interrupt of Module */
			SET_BIT(SREG,GLOBAL_INTERRUPT_ENABLE);    /* Enable Global Interrupt    */
			gu8_Interrupt_Polling_Mode = INTERRUPT;   /* Global variable to store if using Interrupt or Polling */
		}
			else if (pstr_SPI_Confg -> u8_Interrupt_Mode == POLLING)
			{
				gu8_Interrupt_Polling_Mode = POLLING;
			}
				else 
				{
					retval = ERROR_INTERRUPT_POLLING ;				 /* Wrong SPI Not Interrupt or Polling */
				}

		/*** Check for Data Order of SPI ***/
		if(pstr_SPI_Confg -> u8_Data_Order  == MSB) 
		{
			CLR_BIT(SPCR,DORD);
		}
			else if (pstr_SPI_Confg -> u8_Data_Order  == LSB)
			{
				SET_BIT(SPCR,DORD);
			}
				else 
				{
					retval = ERROR_SPI_DATA_ORDER ;		     /* Wrong SPI Not MSB or LSB */
				}

		/*** Check for Clock Polarity ***/
		if(pstr_SPI_Confg -> u8_clk_Polarity  == POLARITY_LOW ) 
		{
			CLR_BIT(SPCR,CPOL);
		}
			else if (pstr_SPI_Confg -> u8_clk_Polarity  == POLARITY_HIGH)
			{
				SET_BIT(SPCR,CPOL);
			}
				else 
				{
					retval = ERROR_SPI_CLK_POLARITY ;		 /* Wrong SPI CLK Not HIGH or LOW Clock */
				}

		/*** Check for Clock Sampling ***/
		if(pstr_SPI_Confg -> u8_clk_Phase  == SAMPLE_LEAD ) 
		{
			CLR_BIT(SPCR,CPHA);
		}
			else if (pstr_SPI_Confg -> u8_clk_Phase  == SAMPLE_TRAIL)
			{
				SET_BIT(SPCR,CPHA);
			}
				else 
				{
					retval = ERROR_SPI_CLK_PHASE ;		    /* Wrong SPI CLK Not in lead or trail */
				}
	}

	else if ( pstr_SPI_Confg ->u8_Master_Mode == SLAVE )
	{
		/* Clear Maser Mode */
		CLR_BIT(SPCR,MSTR);	
	 	DIO_Init_Pin(B4,IN);  /*  SS(PB4)   --> Input   */
	 	DIO_Init_Pin(B5,IN);  /*  MOSI(PB5) --> Input   */
	 	DIO_Init_Pin(B6,OUT); /*  MISO(PB6) --> Output  */
	 	DIO_Init_Pin(B7,IN);  /*  SCK(PB7) --> Input    */

		/*** Check if Interrupt or Polling ***/
		if(pstr_SPI_Confg -> u8_Interrupt_Mode == INTERRUPT) 
		{
			SET_BIT(SPCR,SPIE);                     /* Enable Interrupt of Module */
			SET_BIT(SREG,GLOBAL_INTERRUPT_ENABLE);  /* Enable Global Interrupt    */
			gu8_Interrupt_Polling_Mode = INTERRUPT;   /* Global variable to store if using Interrupt or Polling */
		}
			else if (pstr_SPI_Confg -> u8_Interrupt_Mode == POLLING)
			{
				gu8_Interrupt_Polling_Mode = POLLING;
			}
				else 
				{
					retval = ERROR_INTERRUPT_POLLING ;	   /* Wrong SPI Not Interrupt or Polling */
				}

		/*** Check for Data Order of SPI ***/
		if(pstr_SPI_Confg -> u8_Data_Order  == MSB) 
		{
			CLR_BIT(SPCR,DORD);
		}
			else if (pstr_SPI_Confg -> u8_Data_Order  == LSB)
			{
				SET_BIT(SPCR,DORD);
			}
				else 
				{
					retval = ERROR_SPI_DATA_ORDER ;				 /* Wrong SPI Not MSB or LSB */
				}

		/*** Check for Clock Polarity ***/
		if(pstr_SPI_Confg -> u8_clk_Polarity  == POLARITY_LOW ) 
		{
			CLR_BIT(SPCR,CPOL);
		}
			else if (pstr_SPI_Confg -> u8_clk_Polarity  == POLARITY_HIGH)
			{
				SET_BIT(SPCR,CPOL);
			}
				else 
				{
					retval = ERROR_SPI_CLK_POLARITY ;				 /* Wrong SPI CLK Not HIGH or LOW Clock */
				}

		/*** Check for Clock Sampling ***/
		if(pstr_SPI_Confg -> u8_clk_Phase  == SAMPLE_LEAD ) 
		{
			CLR_BIT(SPCR,CPHA);
		}
			else if (pstr_SPI_Confg -> u8_clk_Phase  == SAMPLE_TRAIL)
			{
				SET_BIT(SPCR,CPHA);
			}
				else 
				{
					retval = ERROR_SPI_CLK_PHASE ;		    /* Wrong SPI CLK Not in lead or trail */
				}
		
	}
	else
	{
		retval = ERROR_SPI_MASTER_SLAVE ;		/* Wrong SPI Not Master or Slave */
	}

	/* Enable SPI Module */
	SET_BIT(SPCR,SPE);
	
	return retval;								/* Return Error Value */
}

/* Description : SPI Send Byte                                                   */
/* Input       : u8_Data "const u8 Data"                                         */
/* Output      : Error Checking                                                  */
uint8_t SPI_Send_Byte(uint8_t const u8_Data)
{
	uint8_t retval = ERROR_OK;
	/* Check if Polling or Interrupt */
	if ( gu8_Interrupt_Polling_Mode == INTERRUPT )
	{
		gu8_Data_SentFlag=0;			/* Clear Global Flag of ISR */
		SPDR = u8_Data;					/* Send DATA */
	}
	else if ( gu8_Interrupt_Polling_Mode == POLLING )
	{
		SPDR = u8_Data;					/* sen DATA */
		while( ! READ_BIT(SPSR,SPIF) );	/* Loop until getting Flag */
	}
	else
	{
		retval = ERROR_INTERRUPT_POLLING;		/* Wrong SPI Not Interrupt or Polling  */
	}

	return retval;
}

/* Description : SPI Receive Byte                                                */
/* Input       : u8_Data "pointer to u8 Data"                                    */
/* Output      : Error Checking                                                  */
uint8_t SPI_Receive_Byte(uint8_t *pu8_Data)
{
	uint8_t retval = ERROR_OK;
	/* Check if Polling or Interrupt */
	if ( gu8_Interrupt_Polling_Mode == INTERRUPT )
	{
		gu8_Data_SentFlag=0;			/* Clear Global Flag of ISR */
		*pu8_Data = SPDR;				/* Read DATA from SPI */
	}
	else if ( gu8_Interrupt_Polling_Mode == POLLING )
	{
		while( ! READ_BIT(SPSR,SPIF) );	/* Loop until getting Flag */
		*pu8_Data = SPDR;				/* Read DATA from SPI */
	}
	else 
	{
		retval = ERROR_INTERRUPT_POLLING;		/* Wrong SPI Not Interrupt or Polling  */
	}
	return retval;
}

/* ISR of SPI */
void __vector_12(void)
{
	gu8_Data_SentFlag = 1;    /* Set the Global variable to ensure of ISR data sent successfully. */
	if(callBack_spi != NULL_PTR )
	{
		callBack_spi();
	}

}

/* Global SPI ISR set callback function */
uint8_t Set_Call_Back_SPI(void (*ptr)(void))
{
	uint8_t retval = ERROR_OK;
	if( ptr != NULL_PTR )
	{
		callBack_spi = ptr;
	}
	else
	{
		retval = ERROR_NULL_POINTER ;
	}
	return retval;
}