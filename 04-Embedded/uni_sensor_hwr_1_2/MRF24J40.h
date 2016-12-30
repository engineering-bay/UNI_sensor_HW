/*
 * MRF24J40.h
 *
 *  Created on: 07.03.2010
 *      Author: di
 */

#ifndef MRF24J40_H_
#define MRF24J40_H_

// short address registers
#define MRF_RXMCR		0x00

#define MRF_TXCRCEN 	7
#define MRF_BBLPBK 		6
#define MRF_ACKEN 		5
#define MRF_MACLPBK 	4
#define MRF_PANCOORD 	3
#define MRF_COORD 		2
#define MRF_RXCRCEN 	1
#define MRF_PROMI		0

#define MRF_PANIDL		0x01
#define MRF_PANIDH		0x02
#define MRF_SADRL		0x03
#define MRF_SADRH		0x04
#define MRF_EADR0		0x05
#define MRF_EADR1		0x06
#define MRF_EADR2		0x07
#define MRF_EADR3		0x08
#define MRF_EADR4		0x09
#define MRF_EADR5		0x0a
#define MRF_EADR6		0x0b
#define MRF_EADR7		0x0c
#define MRF_RXFLUSH		0x0d

#define MRF_RXWRTBLK 	4
#define MRF_CMDONLY 	3
#define MRF_DATAONLY 	2
#define MRF_BCNONLY 	1
#define MRF_RXFLUSHB	0

#define MRF_PACON2		0x18

#define MRF_TXNMTRIG	0x1b
#define MRF_PENDACK 	4
#define MRF_INDIRECT 	3
#define MRF_ACKREQ 		2
#define MRF_SECEN 		1
#define MRF_TXRTS		0

#define MRF_TXSR		0x24
#define MRF_SOFTRST		0x2a

#define MRF_TXSTBL		0x2e

#define MRF_ISRSTS		0x31
#define MRF_SLPIF 		7
#define MRF_WAKEIF 		6
#define MRF_HSYMTMRIF 	5
#define MRF_SECIF 		4
#define MRF_RXIF 		3
#define MRF_GTS2TXIF 	2
#define MRF_GTS1TXIF 	1
#define MRF_TXIF		0

#define MRF_INTMSK		0x32
#define MRF_SLPMSK 		7
#define MRF_WAKEMSK 	6
#define MRF_HSYMTMRMSK 	5
#define MRF_SECMSK 		4
#define MRF_RXMSK 		3
#define MRF_GTS2TXMSK 	2
#define MRF_GTS1TXMSK 	1
#define MRF_TXMSK		0

#define MRF_GPIO		0x33
#define MRF_TRISGPIO	0x34
#define MRF_RFCTL		0x36
#define MRF_RFRST		4

#define MRF_BBREG2		0x3a
#define MRF_BBREG6		0x3e
#define MRF_RSSITHCCA	0x3f

// long address registers
#define MRF_RFCTRL0		0x200
#define MRF_RFCTRL1		0x201
#define MRF_RFCTRL2		0x202
#define MRF_RFCTRL3		0x203
#define MRF_RFCTRL4		0x204
#define MRF_RFCTRL5		0x205
#define MRF_RFCTRL6		0x206
#define MRF_RFCTRL7		0x207
#define MRF_RFCTRL8		0x208
#define MRF_RFSTATE		0x20f

#define MRF_CLKINTCR	0x211
#define MRF_CLKCTRL		0x220

#define EUI_7 0x00    // MAC
#define EUI_6 0x01
#define EUI_5 0x02
#define EUI_4 0x03
#define EUI_3 0x04
#define EUI_2 0x05
#define EUI_1 0x06
#define EUI_0 0x07

/*
//long address registers
#define RFCON0 (0x200)
#define RFCON1 (0x201)
#define RFCON2 (0x202)
#define RFCON3 (0x203)
#define RFCON5 (0x205)
#define RFCON6 (0x206)
#define RFCON7 (0x207)
#define RFCON8 (0x208)
#define SLPCAL0 (0x209)
#define SLPCAL1 (0x20a)
#define SLPCAL2 (0x20b)
#define RFSTATE (0x20f)
#define RSSI (0x210)
#define SLPCON0 (0x211)
#define SLPCON1 (0x220)
#define WAKETIMEL (0x222)
#define WAKETIMEH (0x223)
#define REMCNTL (0x224)
#define REMCNTH (0x225)
#define MAINCNT0 (0x226)
#define MAINCNT1 (0x227)
#define MAINCNT2 (0x228)
#define MAINCNT3 (0x229)
#define TESTMODE (0x22f)

//short address registers for reading
#define READ_RXMCR (0x00)
#define READ_PANIDL (0x02)
#define READ_PANIDH (0x04)
#define READ_SADRL (0x06)
#define READ_SADRH (0x08)
#define READ_EADR0 (0x0A)
#define READ_EADR1 (0x0C)
#define READ_EADR2 (0x0E)
#define READ_EADR3 (0x10)
#define READ_EADR4 (0x12)
#define READ_EADR5 (0x14)
#define READ_EADR6 (0x16)
#define READ_EADR7 (0x18)
#define READ_RXFLUSH (0x1a)
#define READ_ORDER (0x20)
#define READ_TXMCR (0x22)
#define READ_ACKTMOUT (0x24)
#define READ_ESLOTG1 (0x26)
#define READ_SYMTICKL (0x28)
#define READ_SYMTICKH (0x2A)
#define READ_PACON0 (0x2C)
#define READ_PACON1 (0x2E)
#define READ_PACON2 (0x30)
#define READ_TXBCON0 (0x34)
#define READ_TXNCON (0x36)
#define READ_TXG1CON (0x38)
#define READ_TXG2CON (0x3A)
#define READ_ESLOTG23 (0x3C)
#define READ_ESLOTG45 (0x3E)
#define READ_ESLOTG67 (0x40)
#define READ_TXPEND (0x42)
#define READ_WAKECON (0x44)
#define READ_FRMOFFSET (0x46)
#define READ_TXSTAT (0x48)
#define READ_TXBCON1 (0x4A)
#define READ_GATECLK (0x4C)
#define READ_TXTIME (0x4E)
#define READ_HSYMTMRL (0x50)
#define READ_HSYMTMRH (0x52)
#define READ_SOFTRST (0x54)
#define READ_SECON0 (0x58)
#define READ_SECON1 (0x5A)
#define READ_TXSTBL (0x5C)
#define READ_RXSR (0x60)
#define READ_INTSTAT (0x62)
#define READ_INTCON (0x64)
#define READ_GPIO (0x66)
#define READ_TRISGPIO (0x68)
#define READ_SLPACK (0x6A)
#define READ_RFCTL (0x6C)
#define READ_SECCR2 (0x6E)
#define READ_BBREG0        (0x70)
#define READ_BBREG1 (0x72)
#define READ_BBREG2 (0x74)
#define READ_BBREG3 (0x76)
#define READ_BBREG4 (0x78)
#define READ_BBREG6 (0x7C)
#define READ_CCAEDTH (0x7E)

//short address registers for writing

#define WRITE_RXMCR (0x01)
#define WRITE_PANIDL (0x03)
#define WRITE_PANIDH (0x05)
#define WRITE_SADRL (0x07)
#define WRITE_SADRH (0x09)
#define WRITE_EADR0 (0x0B)
#define WRITE_EADR1 (0x0D)
#define WRITE_EADR2 (0x0F)
#define WRITE_EADR3 (0x11)
#define WRITE_EADR4 (0x13)
#define WRITE_EADR5 (0x15)
#define WRITE_EADR6 (0x17)
#define WRITE_EADR7 (0x19)
#define WRITE_RXFLUSH (0x1B)
#define WRITE_ORDER (0x21)
#define WRITE_TXMCR (0x23)
#define WRITE_ACKTMOUT (0x25)
#define WRITE_ESLOTG1 (0x27)
#define WRITE_SYMTICKL (0x29)
#define WRITE_SYMTICKH (0x2B)
#define WRITE_PACON0 (0x2D)
#define WRITE_PACON1 (0x2F)
#define WRITE_PACON2 (0x31)
#define WRITE_TXBCON0 (0x35)
#define WRITE_TXNCON (0x37)
#define WRITE_TXG1CON (0x39)
#define WRITE_TXG2CON (0x3B)
#define WRITE_ESLOTG23 (0x3D)
#define WRITE_ESLOTG45 (0x3F)
#define WRITE_ESLOTG67 (0x41)
#define WRITE_TXPEND (0x43)
#define WRITE_WAKECON (0x45)
#define WRITE_FRMOFFSET (0x47)
#define WRITE_TXSTAT (0x49)
#define WRITE_TXBCON1 (0x4B)
#define WRITE_GATECLK (0x4D)
#define WRITE_TXTIME (0x4F)
#define WRITE_HSYMTMRL (0x51)
#define WRITE_HSYMTMRH (0x53)
#define WRITE_SOFTRST (0x55)
#define WRITE_SECCON0 (0x59)
#define WRITE_SECCON1 (0x5B)
#define WRITE_TXSTBL (0x5D)
#define WRITE_RXSR (0x61)
#define WRITE_INTSTAT (0x63)
#define WRITE_INTCON (0x65)
#define WRITE_GPIO (0x67)
#define WRITE_TRISGPIO (0x69)
#define WRITE_SLPACK (0x6B)
#define WRITE_RFCTL (0x6D)
#define WRITE_SECCR2 (0x6F)
#define WRITE_BBREG0 (0x71)
#define WRITE_BBREG1 (0x73)
#define WRITE_BBREG2 (0x75)
#define WRITE_BBREG3 (0x77)
#define WRITE_BBREG4 (0x79)
#define WRITE_BBREG6 (0x7D)
#define WRITE_CCAEDTH (0x7F)
*/

#define CHANNEL_11 0x03
#define CHANNEL_12 0x13
#define CHANNEL_13 0x23
#define CHANNEL_14 0x33
#define CHANNEL_15 0x43
#define CHANNEL_16 0x53
#define CHANNEL_17 0x63
#define CHANNEL_18 0x73
#define CHANNEL_19 0x83
#define CHANNEL_20 0x93
#define CHANNEL_21 0xa3
#define CHANNEL_22 0xb3
#define CHANNEL_23 0xc3
#define CHANNEL_24 0xd3
#define CHANNEL_25 0xe3
#define CHANNEL_26 0xf3

#endif /* MRF24J40_H_ */
