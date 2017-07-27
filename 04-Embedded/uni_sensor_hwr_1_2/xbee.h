/*
 * xbee.h
 *
 *  Created on: 06 сент. 2015 г.
 *      Author: werwolf
 */

#ifndef XBEE_H_
#define XBEE_H_

#include <stdint.h>

//=== XBee module AT defines ========================================
#define ENTER_CM				"+++"
#define ATMY_READ				"ATMY\r"
//#define ATMY_SET				"ATMY 03\r"  // не работает!
#define PANID_READ				"ATID\r"
#define NI_READ					"ATNI\r"
#define NI_SET					"ATNI D_temp_balkon\r"
#define POWER_LEVEL_READ		"ATPL\r"
#define POWER_LEVEL_SET0		"ATPL 0\r"
#define SLEEP_MODE_READ			"ATSM\r"
#define SLEEP_MODE_SET_NOSLEEP	"ATSM 0\r"
#define SLEEP_MODE_SET_PIN_HIB	"ATSM 1\r"
#define WRITE_TO_FLASH			"ATWR\r"

//=== XBee module API defines ========================================
#define API_FRAME_DELIMITER					0x7E
#define API_IDENTIFIER_ADV_MODEM_STATUS		0x8C
#define API_IDENTIFIER_ZBEE_TX_REQUEST		0x10
#define API_IDENTIFIER_ZBEE_TX_STATUS		0x8B
#define API_IDENTIFIER_ZBEE_RX_PACKET		0x90

#define API_FRAMEID_NORESPONSE				0x00

//=== Function prototypes ============================================
//void XBEE_senddata(unsigned char own_addr);
void XBEE_senddata_api(void);
uint8_t crc_calc(uint8_t *ptr, int len);
uint8_t crc_check(uint8_t *ptr, int len);
void XBEE_addr_request(void);

#endif /* XBEE_H_ */


