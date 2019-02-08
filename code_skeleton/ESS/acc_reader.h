#include <stdint.h>

struct acc3 {
	int16_t x;
	int16_t y;
	int16_t z;
};

typedef struct acc3 acc3_t;

// Initialize accelerometer
// return 1 if error otherwise 0
uint16_t AccInit(void);

// Obtain a reading in angles
void AccRead(acc3_t * reading);
