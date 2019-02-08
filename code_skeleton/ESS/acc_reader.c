#include "acc_reader.h"
#include "spi_driver.h"
#include "stdio.h"

static const uint8_t CTRL_REG1 = 0x20;
static const uint8_t OUT_X_H = 0x29;
static const uint8_t OUT_X_L = 0x28;
static const uint8_t OUT_Y_H = 0x2B;
static const uint8_t OUT_Y_L = 0x2A;
static const uint8_t OUT_Z_H = 0x2D;
static const uint8_t OUT_Z_L = 0x2C;

void AccInit(void) {
	SPIAcc_Init();
	SPIAcc_SendByte(CTRL_REG1, 0x87);
}

static int8_t compute_x_tilt(int16_t x_axis) {
	// 100 -> 600 >> normal
	// 10K ~ 45 C degrees
	if(x_axis <= -10000) {
		return -46;
	}
	if(x_axis >= 10000) {
		return 46;
	}
	if(100 <= x_axis && x_axis <= 600) {
		return 0;
	}

	return x_axis / 217; // approximating a degree to ~ 200
}

static int8_t compute_y_tilt(int16_t y_axis) {
	// -400 -> 100 >> normal
	// 10L | 45 degrees
	if(y_axis <= -10000) {
		return -46;
	}
	if(y_axis >= 10000) {
		return 46;
	}
	if(-400 <= y_axis && y_axis <= 100) {
		return 0;
	}

	return y_axis / 217; // approximating a degree to ~ 200
}

void AccRead(acc3_t * reading) {
	uint8_t data_xh = SPIAcc_GetByte(OUT_X_H);
	uint8_t data_xl = SPIAcc_GetByte(OUT_X_L);
	int16_t x_axis = (data_xh << 8) + data_xl;

	uint8_t data_yh = SPIAcc_GetByte(OUT_Y_H);
	uint8_t data_yl = SPIAcc_GetByte(OUT_Y_L);
	int16_t y_axis = (data_yh << 8) + data_yl;

	uint8_t data_zh = SPIAcc_GetByte(OUT_Z_H);
	uint8_t data_zl = SPIAcc_GetByte(OUT_Z_L);
	int16_t z_axis = (data_zh << 8) + data_zl;

	printf("x: %d || y: %d || z: %d\n", x_axis, y_axis, z_axis);
	uint8_t x_tilt = compute_x_tilt(x_axis);
	uint8_t y_tilt = compute_y_tilt(y_axis);
	
	reading->x = x_tilt;
	reading->y = y_tilt;
	reading->z = 0; // TODO
}
