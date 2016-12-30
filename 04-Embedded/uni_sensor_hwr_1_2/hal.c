/*
 * File name: 		hal.c
 * Description:		Hardware Abstraction Layer for UNI_SENSOR_HWR_1_2
 * Created on:		11 дек. 2016 г.
 * Author: 			werwolf
 */
#include <msp430.h>
#include <uni_sensor.h>
#include <hal.h>

extern unsigned char TX_buffer[], RX_buffer[];
extern int TX_index, RX_index, TX_length;

int UART_tx_data(unsigned char *ptr, int datalength)
{
	int i;

	if(datalength > TX_BUFFER_SIZE)
	{
		return -1;
	}
	else
	{
		for(i = 0; i < datalength; i++)
		{
			TX_buffer[i] = ptr[i];
		}
		TX_length = datalength;
		TX_index = 0;
		IFG2 |= UCA0TXIFG;		// Set UART TX interrupt flag
		return 0;
	}
}

unsigned char * UART_rx_data(long timeout, int datalength)
{
	long to = 0;
	while(to < timeout && RX_index < datalength)
	{
		__delay_cycles(100);
		to += 100;
	}
	if(RX_index == 0) return 0;
	else
	{
		return RX_buffer;
	}
}

unsigned char SPI_master_write(unsigned char dbyte)
{
	unsigned char res = 0xFF;
	UCB0TXBUF = dbyte;
	while((IFG2 & UCB0RXIFG) != UCB0RXIFG); //wait for byte received
	res = UCB0RXBUF;
	//IFG2 &= ~UCB0RXIFG;
	return res;
}

int SPI_tx_data(unsigned char *ptr, int datalength)
{
	int i;

	if(datalength > TX_BUFFER_SIZE)
	{
		return -1;
	}
	else
	{
		for(i = 0; i < datalength; i++)
		{
			TX_buffer[i] = ptr[i];
		}
		TX_length = datalength;
		TX_index = 0;
		RX_index = 0;
		SPI_CS_control(0);		// set #CS_RF low
		IFG2 |= UCB0TXIFG;		// Set UART TX interrupt flag
		return 0;
	};
}

void SPI_CS_control(int command)
{
	if(command == 0) P2OUT &= ~BIT1;		// set #CS_RF low
	else P2OUT |= BIT1;					// set #CS_RF high
}
