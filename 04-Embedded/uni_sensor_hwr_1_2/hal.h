/*
 * hal.h
 *
 *  Created on: 11 дек. 2016 г.
 *      Author: werwolf
 */

#ifndef HAL_H_
#define HAL_H_

int UART_tx_data(unsigned char *ptr, int datalength);
unsigned char * UART_rx_data(long timeout, int datalength);
unsigned char SPI_master_write(unsigned char dbyte);
int SPI_tx_data(unsigned char *ptr, int datalength);
void SPI_CS_control(int command);

#endif /* HAL_H_ */
