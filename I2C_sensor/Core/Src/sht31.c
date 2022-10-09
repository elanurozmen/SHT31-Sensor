/*
 * sht31.c
 *
 *  Created on: 26 Eyl 2022
 *      Author: Elanur
 */
#include "sht31.h"
#include "main.h"
I2C_HandleTypeDef hi2c1;

uint8_t buf[2], rxBuf[6];
int32_t tempRaw;
uint32_t humRaw;
uint16_t temp, hum;
float temperature, humidity;
int freqint;

void startSingleShot(char clockStretching, int freq) {
	if (clockStretching == 'e') {
		buf[0] = 0x2C;
		switch (freq) {
		case 0:
			buf[1] = 0x06;
			break;
		case 1:
			buf[1] = 0x0D;
			break;
		case 2:
			buf[1] = 0x01;
			break;
		}
	} else if (clockStretching == 'd') {
		buf[0] = 0x24;
		switch (freq) {
		case 3:
			buf[1] = 0x00;
			break;
		case 4:
			buf[1] = 0x0B;
			break;
		case 5:
			buf[1] = 0x16;
			break;
		}
		HAL_I2C_Master_Transmit(&hi2c1, SHT31_ID, buf, sizeof(buf), 50);
		HAL_Delay(1000);
	}
}

void startPeriodicShot(int mps, int freq) {
	if (mps == 0.5) {
		buf[0] = 0x20;
		switch (freq) {
		case 0:
			buf[1] = 0x32;
			break;
		case 1:
			buf[1] = 0x24;
			break;
		case 2:
			buf[1] = 0x2F;
			break;
		}
	} else if (mps == 1) {
		buf[0] = 0x21;
		switch (freq) {
		case 3:
			buf[1] = 0x30;
			break;
		case 4:
			buf[1] = 0x26;
			break;
		case 5:
			buf[1] = 0x2D;
			break;
		}
	} else if (mps == 2) {
		buf[0] = 0x22;
		switch (freq) {
		case 6:
			buf[1] = 0x36;
			break;
		case 7:
			buf[1] = 0x20;
			break;
		case 8:
			buf[1] = 0x2B;
			break;
		}
	} else if (mps == 4) {
		buf[0] = 0x23;
		switch (freq) {
		case 9:
			buf[1] = 0x34;
			break;
		case 10:
			buf[1] = 0x22;
			break;
		case 11:
			buf[1] = 0x29;
			break;
		}
	} else if (mps == 10) {
		buf[0] = 0x27;
		switch (freq) {
		case 12:
			buf[1] = 0x37;
			break;
		case 13:
			buf[1] = 0x21;
			break;
		case 14:
			buf[1] = 0x2A;
			break;
		}
	} else {
		buf[0] = 0x22;
		buf[1] = 0x20;
	}
	HAL_I2C_Master_Transmit(&hi2c1, SHT31_ID, buf, sizeof(buf), 50);
	HAL_Delay(1000);
}
void statusRegister() {
	buf[0] = SHT31_STATUS_REGISTER_BYTE1;
	buf[1] = SHT31_STATUS_REGISTER_BYTE2;
	HAL_I2C_Master_Transmit(&hi2c1, SHT31_ID, buf, sizeof(buf), 50);
}
void clearStatusRegister() {
	buf[0] = SHT31_CLEAR_STATUS_REGISTER_BYTE1;
	buf[1] = SHT31_CLEAR_STATUS_REGISTER_BYTE2;
	HAL_I2C_Master_Transmit(&hi2c1, SHT31_ID, buf, sizeof(buf), 50);
}
void breakCommand() {
	buf[0] = SHT31_BREAK_COMMAND_BYTE1;
	buf[1] = SHT31_BREAK_COMMAND_BYTE2;
	HAL_I2C_Master_Transmit(&hi2c1, SHT31_ID, buf, sizeof(buf), 50);
	HAL_Delay(1000);
}

void softReset() {
	buf[0] = SHT31_SOFT_RESET_BYTE1;
	buf[1] = SHT31_SOFT_RESET_BYTE2;
	HAL_I2C_Master_Transmit(&hi2c1, SHT31_ID, buf, sizeof(buf), 50);
	HAL_Delay(1000);
}

void readSingleShot() {
	HAL_I2C_Master_Receive(&hi2c1, SHT31_ID, rxBuf, sizeof(rxBuf), 500);
	HAL_Delay(1000);
	calcuteTempAndHum(rxBuf);
}
void readPeriodicShot() {
	buf[0] = SHT31_READ_PERIODIC_SHOT_BYTE1;
	buf[1] = SHT31_READ_PERIODIC_SHOT_BYTE2;
	HAL_I2C_Master_Transmit(&hi2c1, SHT31_ID, buf, sizeof(buf), 500);
	HAL_I2C_Master_Receive(&hi2c1, SHT31_ID, rxBuf, sizeof(rxBuf), 500);
	HAL_Delay(1000);
	calcuteTempAndHum(rxBuf);
}
void heaterEnable(uint8_t *buf, int size) {
	buf[0] = SHT31_HEATER_ENABLE_BYTE1;
	buf[1] = SHT31_HEATER_ENABLE_BYTE2;
	HAL_I2C_Master_Transmit(&hi2c1, SHT31_ID, buf, size, 50);
	HAL_Delay(35000);
}

void heaterDisabled(uint8_t *buf, int size) {
	buf[0] = SHT31_HEATER_DISABLED_BYTE1;
	buf[1] = SHT31_HEATER_DISABLED_BYTE2;
	HAL_I2C_Master_Transmit(&hi2c1, SHT31_ID, buf, size, 50);
	HAL_Delay(25000);
}
/*Temperature and Humidity Calculations */
void calcuteTempAndHum(uint8_t *rxData) {
	tempRaw = ((rxData[0] << 8) | rxData[1]);
	humRaw = ((rxData[3] << 8) | rxData[4]);
	/* Calculate actual value */
	temp = ((4375 * tempRaw) >> 14) - 4500;
	hum = (625 * humRaw) >> 12;
	temperature = (float) (temp / 100);
	humidity = hum / 100;
}

