#include <msp430.h>
#include <stdint.h>
#include <string.h>
#include "xbee.h"
#include "user_cfg.h"

//=== XBee user protocol defines ====================================
#define COMMAND_SENSOR_POLL	0xA1

//int len;
char * uart_tx_buffer;
extern uint8_t OwnAddress;
extern unsigned char RX_buffer[];
extern unsigned char TX_buffer[];
extern int TX_index, RX_index, TX_length;
extern float param[];
extern float tmp1[], tmp2[];
extern int window_size;
//uint8_t CoordinatorAddress[8] = {0,0,0,0,0,0,0,0};

void XBEE_senddata_api()
{
	int i;

	P2OUT &= ~BIT2; // get Xbee module out of sleep mode;
	// пересчитываем значения параметров
	param[0] = 0;
	param[3] = 0;
	for(i = 0; i < window_size; i++)
	{
		param[0] += (tmp1[i]/window_size);
		param[3] += (tmp2[i]/window_size);
	}
	/*while((P2IN & BIT6) != BIT6)
	{

	}*/
	__delay_cycles(100000); // wait for Xbee wake up

	TX_buffer[0] = API_FRAME_DELIMITER;
	TX_buffer[1] = 0x00; // length MSB
	TX_buffer[2] = 0x26; // length LSB
	TX_buffer[3] = API_IDENTIFIER_ZBEE_TX_REQUEST;
	TX_buffer[4] = API_FRAMEID_NORESPONSE;
	/* 64-bit destination address*/
	TX_buffer[5] = 0x00;
	TX_buffer[6] = 0x00;
	TX_buffer[7] = 0x00;
	TX_buffer[8] = 0x00;
	TX_buffer[9] = 0x00;
	TX_buffer[10] = 0x00;
	TX_buffer[11] = 0x00;
	TX_buffer[12] = 0x00;
	/* 16-bit destination network address*/
	TX_buffer[13] = 0x00;
	TX_buffer[14] = 0x00;
    /* Broadcast radius */
	TX_buffer[15] = 0x00;
	/* Options: 0x01 = Disable ACK, 0x02 = Disable Network Address Discovery. All other bits must be set to 0. */
	TX_buffer[16] = 0x00;
	/* RF data */
	TX_buffer[17] = OwnAddress;
	TX_buffer[18] = BASE_ADDRESS;
	TX_buffer[19] = COMMAND_SENSOR_POLL;
	TX_buffer[20] = 'T';
	TX_buffer[21] = *((unsigned char *)&param[0] + 0);
	TX_buffer[22] = *((unsigned char *)&param[0] + 1);
	TX_buffer[23] = *((unsigned char *)&param[0] + 2);
	TX_buffer[24] = *((unsigned char *)&param[0] + 3);
	TX_buffer[25] = 'D';
	TX_buffer[26] = *((unsigned char *)&param[1] + 0);
	TX_buffer[27] = *((unsigned char *)&param[1] + 1);
	TX_buffer[28] = *((unsigned char *)&param[1] + 2);
	TX_buffer[29] = *((unsigned char *)&param[1] + 3);
	TX_buffer[30] = 'D';
	TX_buffer[31] = *((unsigned char *)&param[2] + 0);
	TX_buffer[32] = *((unsigned char *)&param[2] + 1);
	TX_buffer[33] = *((unsigned char *)&param[2] + 2);
	TX_buffer[34] = *((unsigned char *)&param[2] + 3);
	TX_buffer[35] = 'V';
	TX_buffer[36] = *((unsigned char *)&param[3] + 0);
	TX_buffer[37] = *((unsigned char *)&param[3] + 1);
	TX_buffer[38] = *((unsigned char *)&param[3] + 2);
	TX_buffer[39] = *((unsigned char *)&param[3] + 3);
	TX_buffer[40] = '$';
	/* Checksum */
	TX_buffer[41] = crc_calc((uint8_t *) (TX_buffer + 3), 38);

	TX_index = 0;
	RX_index = 0;
	TX_length = 42;
	IFG2 |= UCA0TXIFG; // set TX interrupt flag to start transmission
}

uint8_t crc_calc(uint8_t *ptr, int len)
{
  uint8_t sum = 0;
  int i;

  for(i = 0; i < len; i++)
  {
    sum += ptr[i];
  }
  return (0xFF - sum);
}

uint8_t crc_check(uint8_t *ptr, int len)
{
  int i;
  uint8_t sum;

  for(i = 0; i <= len; i++)
  {
    sum += ptr[i];
  }
  if(sum == 0xFF) return 1;
  else return 0;
}

void XBEE_addr_request(void)
{

  TX_buffer[0] = 0x7E; // delimiter
  TX_buffer[1] = 0x00; // length MSB
  TX_buffer[2] = 0x04; // length LSB
  TX_buffer[3] = 0x08; // API identifier "AT command"
  TX_buffer[4] = 0x53; // frame ID
  TX_buffer[5] = 'S';  // AT-command
  TX_buffer[6] = 'H';  // AT-command
  TX_buffer[7] = crc_calc((uint8_t *) (TX_buffer + 3), 4); // checksum

  TX_length = 8;
  TX_index = 0;
  RX_index = 0;
  IFG2 |= UCA0TXIFG; // set TX interrupt flag to start transmission
}
