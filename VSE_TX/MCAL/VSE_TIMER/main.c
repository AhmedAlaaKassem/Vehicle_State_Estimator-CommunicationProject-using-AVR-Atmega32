*
 * main.c
 *
 *  Created on: ??þ/??þ/????
 *      Author: Eng Mohamed Hamdy
 */

#include <DIO/DIO_Interface.h>
#include "UART_interface.h"
#include <util/delay.h>

#include "Bit_Math.h"
#include "Compiler.h"

uint8_t Data=NULL;
uint8_t Temp_Data=NULL;

void callback(uint8_t data)
{
	static uint8_t index  = 0;
	Data = data;
	index++;
	if(Data == 13)
	{
		DIO_Write_Port(DIO_PORTA, Temp_Data);
		Temp_Data = 0;
	}
	else if ((Data != '\0')&&((Data >= '0')||(Data <= '9')))
	{
		Temp_Data = (Temp_Data * 10) + (Data-48);
		Data = '\0';
	}
	else if ((Data != '\0')&&(Data >= 'a'))
	{
		Temp_Data = (Temp_Data * 10) + (Data);
		Data = '\0';
	}
}
int main(void)
{
	UART_cfg_s Cfg;
	Cfg.u16_BaudRate = BAUDRATE_9600;
	Cfg.u8_CharSize  = CHAR_8B;
	//Cfg.u8_Interrupt_Mode = POLLING;
	Cfg.u8_Interrupt_Mode = INTERRUPT;
	Cfg.u8_Parity = Disabled_PARITY;
	Cfg.u8_SpeedMode = NORMAL_SPEED;
	Cfg.u8_StopSize = STOP_1B;
	Cfg.u8_Sync = ASYNCHRONOUS;

	UART_Init(&Cfg);
	UART_callBackRx(callback);
/*
	DIO_INIT_Pin(DIO_PB4, OUTPUT_PIN);
	DIO_INIT_Pin(DIO_PB5, OUTPUT_PIN);
	DIO_INIT_Pin(DIO_PB6, OUTPUT_PIN);
	DIO_INIT_Pin(DIO_PB7, OUTPUT_PIN);
	*/
	DIO_INIT_Port(DIO_PORTA, OUTPUT_PORT);
	//DIO_Write_Port(DIO_PORTA, HIGH_PORT);


	while (1)
	{
		/*
		if(Data == 13)
		{
			DIO_Write_Port(DIO_PORTA, Temp_Data);
		}
		else
		{
			Temp_Data = (Temp_Data * 10) + (Data);
			//Data = 0;
		}
		*/
		//DIO_Write_Port(DIO_PORTA, Data);
		/*
		if(Data[0]=='a')
		{
			DIO_Write_Pin(DIO_PB4, HIGH_PIN);
			//_delay_ms(1000);
			//DIO_Write_Pin(DIO_PB4, LOW_PIN);
		}
		if(Data[1]=='b')
		{
			DIO_Write_Pin(DIO_PB5, HIGH_PIN);
			//_delay_ms(1000);
			//DIO_Write_Pin(DIO_PB5, LOW_PIN);
		}
		if(Data[2]=='c')
		{
			DIO_Write_Pin(DIO_PB6, HIGH_PIN);
			//_delay_ms(1000);
			//DIO_Write_Pin(DIO_PB6, LOW_PIN);
		}
		if(Data[3]=='d')
		{
			DIO_Write_Pin(DIO_PB7, HIGH_PIN);
			//_delay_ms(1000);
			//DIO_Write_Pin(DIO_PB7, LOW_PIN);
		}
		 */

	}

return 0;
}
