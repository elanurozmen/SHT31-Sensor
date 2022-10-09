/*
 * sht31.h
 *
 *  Created on: 26 Eyl 2022
 *      Author: Elanur
 */

#ifndef INC_SHT31_H_
#define INC_SHT31_H_

#include <stdint.h>

#define SHT31_ID								(0x44 << 1)
#define SHT31_SOFT_RESET_BYTE1					0x30
#define SHT31_SOFT_RESET_BYTE2  				0xA2
#define SHT31_START_PERIODIC_SHOT     			0x21
#define SHT31_START_PERIODIC_SHOT_MEDIUM		0x26
#define SHT31_READ_PERIODIC_SHOT_BYTE1			0xE0
#define SHT31_READ_PERIODIC_SHOT_BYTE2			0x00
#define SHT31_HEATER_ENABLE_BYTE1				0x30
#define SHT31_HEATER_ENABLE_BYTE2				0x6D
#define SHT31_HEATER_DISABLED_BYTE1				0x30
#define SHT31_HEATER_DISABLED_BYTE2				0x66
#define SHT31_BREAK_COMMAND_BYTE1				0x30
#define SHT31_BREAK_COMMAND_BYTE2				0x93
#define SHT31_STATUS_REGISTER_BYTE1				0xF3
#define SHT31_STATUS_REGISTER_BYTE2				0x2D
#define SHT31_CLEAR_STATUS_REGISTER_BYTE1		0x30
#define SHT31_CLEAR_STATUS_REGISTER_BYTE2		0x41

void startSingleShot(char clockStretching, int freq);
void startPeriodicShot(int mps, int freq);
void statusRegister(void);
void clearStatusRegister(void);
void breakCommand(void);
void softReset(void);
void readSingleShot(void);
void readPeriodicShot(void);
void heaterEnable(uint8_t *buf, int size);
void heaterDisabled(uint8_t *buf, int size);
void calcuteTempAndHum(uint8_t *rx_data);


#endif /* INC_SHT31_H_ */
