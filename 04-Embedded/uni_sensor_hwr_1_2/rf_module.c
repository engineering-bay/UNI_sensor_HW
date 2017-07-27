/*
 * rf_module.c
 *
 *  Created on: 11 дек. 2016 г.
 *      Author: werwolf
 */
#include <msp430.h>
#include <uni_sensor.h>
#include <hal.h>
#include <MRF24J40.h>
#include <zigbee.h>
#include <user_cfg.h>
#include "xbee.h"


extern uint8_t OwnAddress;
extern unsigned char RX_buffer[];
extern unsigned char TX_buffer[];
extern int TX_index, RX_index, TX_length;
extern float param[];
extern float tmp1[], tmp2[];
extern int window_size;
extern unsigned char RFType;

char * uart_tx_buffer;
unsigned char ZigbeeMac[8] = {0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18};


void ZigbeeModuleInit(void)
{
	RFType = RFMODULETYPE_XBEE;
	InitXBee();
	/*RFType = GetRFModuleType();
	switch(RFType)
	{
		case RFMODULETYPE_XBEE:  // if XBee module used
			InitXBee();
			break;

		case RFMODULETYPE_MRF24:  // if MRF24 module used
			InitMRF24();
			break;
	}*/
}

int GetRFModuleType(void)
{
	unsigned char *ptr;
	unsigned char str_xbee[8] = {0x7E, 0x00, 0x04, 0x08, 0x01, 0x4E, 0x49, 0x5F}; // XBee module API status request
	// 0001_0010 0010_0100
	//unsigned char str_mrf24[] = {0x24, 0x00}; // MRF24 module
	UART_tx_data(str_xbee, 8);
	ptr = UART_rx_data(30000, 6);
	if(ptr != 0)
	{
		return RFMODULETYPE_XBEE;			// XBee module found!
	}
	else
	{
		if(MRF_read_s(0x11) == 0x1C)
		{
			return RFMODULETYPE_MRF24;
		}
		else
		{
			return RFMODULETYPE_UNKNOWN;
		}
	}
}

// чтение по короткому адресу
unsigned char MRF_read_s(unsigned char addr)
{
	unsigned char data;
	SPI_CS_control(0);
	__delay_cycles(1);
	SPI_master_write(((addr<<1) & 0x7e));
	data = SPI_master_write(0);
	SPI_CS_control(1);
	return data;
}

// запись по короткому адресу
void MRF_write_s(unsigned char addr, unsigned char data)
{
	SPI_CS_control(0);
	__delay_cycles(1);
	SPI_master_write(((addr<<1) & 0x7f) | 0x01);
	SPI_master_write(data);
	SPI_CS_control(1);
}

// чтение по длинному адресу
unsigned char MRF_read_l(int addr)
{
	unsigned char data;
	SPI_CS_control(0);
	__delay_cycles(5);
	SPI_master_write(((addr>>3) & 0x7f) | 0x80);
	SPI_master_write(((addr<<5) & 0xe0));
	data = SPI_master_write(0);
	SPI_CS_control(1);
	return data;
}

// запись по длинному адресу
void MRF_write_l(int addr, unsigned char data)
{
	SPI_CS_control(0);
	SPI_master_write(((addr>>3) & 0x7f) | 0x80);
	SPI_master_write(((addr<<5) & 0xe0) | 0x1f);
	SPI_master_write(data);
	SPI_CS_control(1);
}

void MRF_setChannel(unsigned char Channel)
{
    MRF_write_l(MRF_RFCTRL0, Channel<<4);
    MRF_write_s(MRF_RFCTL,0x04);
    MRF_write_s(MRF_RFCTL,0x00);
}

void InitXBee(void)
{
	;
}

void InitMRF24(void)
{
	int i;
	// ѕрограммный сброс трансивера.
	MRF_write_s(MRF_SOFTRST, 0x07);
	while (MRF_read_s(MRF_SOFTRST) & 0x07);

	//PACON2 = 0x98, Initialize FFOEN=1 and TXONTS = 0x6
	MRF_write_s(MRF_PACON2, 0x98);
	//Initialize RFSTBL = 0x9
	MRF_write_s(MRF_TXSTBL,0x95);
	//Initialize VCOOPT=1
	MRF_write_l(MRF_RFCTRL1, 0x01);
	//Enable PLL
	MRF_write_l(MRF_RFCTRL2, 0x80);
	//Initialize TXFIL=1, 20MRECVR=1
	MRF_write_l(MRF_RFCTRL6, 0x90);
	//Initialize SLPCLKSEL = 0x2 (100KHz internal oscialltor)
	MRF_write_l(MRF_RFCTRL7, 0x80);
	//Initialize RFVCO =1
	MRF_write_l(MRF_RFCTRL8, 0x10);
	//Initialize CLKOUTEN=1 and SLPCLKDIV = 0x01
	MRF_write_l(MRF_CLKCTRL, 0x21);
	//Set CCA mode to ED
	MRF_write_s(MRF_BBREG2, 0x80);
	//Set CCA-ED Threshold
	MRF_write_s(MRF_RSSITHCCA, 0x60);
	//Set appended RSSI value to RX FIFO
	MRF_write_s(MRF_BBREG6, 0x40);
	//INTCON (0x32) = 0xF6 - Enables only TXNIF and RXIF interrupts
	MRF_write_s(MRF_INTMSK, 0xF6);
	//set operating channel as channel 12
	MRF_setChannel(CHANNEL_19);
	__delay_cycles(50000);	// wait 50ms
	//Wait until the RFSTATE machine indicates RX state
	while((MRF_read_l(MRF_RFSTATE) & 0xa0) != 0xa0);
	// Program the short MAC Address, 0xffff
	//load the short address of the device with 0xffff which means that it will be ignored upon receipt
	MRF_write_s(MRF_SADRL, ZIGBEE_SHORTADDR_LOW);
	MRF_write_s(MRF_SADRH, ZIGBEE_SHORTADDR_HIGH);
	//load the pan address also with YOUR_PAN_ID;
	MRF_write_s(MRF_PANIDL, ZIGBEE_PANID_LOW);
	MRF_write_s(MRF_PANIDH, ZIGBEE_PANID_HIGH);
	//load Associated coordinator short address
	MRF_write_l(0x238, ZIGBEE_COORDSADDR_LOW);
	MRF_write_l(0x239, ZIGBEE_COORDSADDR_HIGH);
	//load Associated coordinator extended address
	MRF_write_l(0x230, ZIGBEE_COORDEADDR0);
	MRF_write_l(0x231, ZIGBEE_COORDEADDR1);
	MRF_write_l(0x232, ZIGBEE_COORDEADDR2);
	MRF_write_l(0x233, ZIGBEE_COORDEADDR3);
	MRF_write_l(0x234, ZIGBEE_COORDEADDR4);
	MRF_write_l(0x235, ZIGBEE_COORDEADDR5);
	MRF_write_l(0x236, ZIGBEE_COORDEADDR6);
	MRF_write_l(0x237, ZIGBEE_COORDEADDR7);

	//Program Long MAC Address
	for(i=0;i<8;i++)
	{
		MRF_write_s(MRF_EADR0+i, 0x11);
	}

	// Automatic acknowledgement is not transmitted for received packets by TXMAC
	//ERRpkt mode bit is set to 0
	//Promiscuous packet mode bit is set to 1
	MRF_write_s(MRF_RXMCR, 0x00);
}

unsigned char MRF_sendPacket(unsigned char ch)
{
	unsigned char stat = 0x00;

	MRF_write_l(0x000, 22); // packet header length //
	MRF_write_l(0x001, 22); // total packet length (not including the FCS/CRC or length) //
	MRF_write_l(0x002, 0b01100001); /* last byte of packet (the LSB of the frame control p112 of IEEE 802.15.4-2003) */
	MRF_write_l(0x003, 0xCC); /* first byte of packet (the MSB of the frame control p112 of IEEE 802.15.4-2003), use long addresses for both cases */
	//MRF_write_l(0x004, 0x01); // IEEE sequence number //
	MRF_write_l(0x004, ZIGBEE_PANID_LOW); // PANID - broadcast //
	MRF_write_l(0x005, ZIGBEE_PANID_HIGH); // PANID - broadcast //
	MRF_write_l(0x006, ZIGBEE_COORDEADDR0); // Dest Address LSB //
	MRF_write_l(0x007, ZIGBEE_COORDEADDR1); // Dest Address  //
	MRF_write_l(0x008, ZIGBEE_COORDEADDR2); // Dest Address  //
	MRF_write_l(0x009, ZIGBEE_COORDEADDR3); // Dest Address  //
	MRF_write_l(0x00a, ZIGBEE_COORDEADDR4); // Dest Address  //
	MRF_write_l(0x00b, ZIGBEE_COORDEADDR5); // Dest Address  //
	MRF_write_l(0x00c, ZIGBEE_COORDEADDR6); // Dest Address  //
	MRF_write_l(0x00d, ZIGBEE_COORDEADDR7); // Dest Address MSB //
	MRF_write_l(0x00e, 0x11); // Source Address LSB //
	MRF_write_l(0x01f, 0x11); // Source Address  //
	MRF_write_l(0x010, 0x11); // Source Address  //
	MRF_write_l(0x011, 0x11); // Source Address  //
	MRF_write_l(0x012, 0x11); // Source Address  //
	MRF_write_l(0x013, 0x11); // Source Address  //
	MRF_write_l(0x014, 0x11); // Source Address  //
	MRF_write_l(0x015, 0x11); // Source Address MSB //
	MRF_write_l(0x016, 0x55); // data byte //
	MRF_write_l(0x017, 0xAA); // data byte //

	MRF_write_s(0x36, 0x06);  // set reset RF state machine
	MRF_write_s(0x36, 0x02);  // clear reset RF state machine & TX mode
	__delay_cycles(200);
	MRF_write_s(MRF_TXNMTRIG, 0x01);  //force transmit
	//while((P2IN & BIT6) == 0x00);
	//__delay_cycles(100000);
	//stat = MRF_read_s(0x24);
	while((MRF_read_s(0x31) & 0x01) != 0x01);
	stat = MRF_read_s(0x24);
	MRF_write_s(0x36, 0x01);  // RX mode
	return stat;
}
