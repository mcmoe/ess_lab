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
static const uint8_t WHO_AM_I = 0xF;

uint16_t test_accelerometer() {
	uint8_t who_am_i = SPIAcc_GetByte(WHO_AM_I);
	//printf("who am i[%#02x]: %#02x\n", WHO_AM_I, who_am_i);
	return who_am_i != 0x3f;
}

uint16_t AccInit(void) {
	SPIAcc_Init();
	SPIAcc_SendByte(CTRL_REG1, 0x87);	
	return test_accelerometer();
}

/*
* Assuming a measurement using a 16bit ADC
* Then we can calculate 360° = (2^16)
* 
*/
static int8_t compute_tilt(int16_t axis) {
	return (int32_t) 360 * axis / 65536;
}

static int16_t read_axis(uint8_t high, uint8_t low) {
	uint8_t data_h = SPIAcc_GetByte(high);
	uint8_t data_l = SPIAcc_GetByte(low);
	return (data_h << 8) + data_l;
}

void AccRead(acc3_t * reading) {
	int16_t x_axis = read_axis(OUT_X_H, OUT_X_L);
	int16_t y_axis = read_axis(OUT_Y_H, OUT_Y_L);
	int16_t z_axis = read_axis(OUT_Z_H, OUT_Z_L);

	int8_t x_tilt = compute_tilt(x_axis);
	int8_t y_tilt = compute_tilt(y_axis);
  int8_t z_tilt = compute_tilt(z_axis);
	printf("\nxa: %d || ya: %d || za: %d", x_tilt, y_tilt, z_tilt);

	reading->x = x_tilt;
	reading->y = y_tilt;
	reading->z = z_tilt;
}
