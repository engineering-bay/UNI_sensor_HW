/*
 * uni_sensor.h
 *
 *  Created on: 10 дек. 2016 г.
 *      Author: werwolf
 */

#ifndef UNI_SENSOR_H_
#define UNI_SENSOR_H_

#define WINDOW_SIZE		15
#define TX_BUFFER_SIZE	50
#define RX_BUFFER_SIZE	50


extern void init_mcu(void);
extern void ZigbeeModuleInit(void);
extern int GetRFModuleType(void);
unsigned char MRF_read_s(unsigned char addr);
void MRF_write_s(unsigned char addr, unsigned char data);
unsigned char MRF_read_l(int addr);
void MRF_write_l(int addr, unsigned char data);
void MRF_setChannel(unsigned char Channel);
extern void InitXBee(void);
extern void InitMRF24(void);
unsigned char MRF_sendPacket(unsigned char ch);

#endif /* UNI_SENSOR_H_ */
