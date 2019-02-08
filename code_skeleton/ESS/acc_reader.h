#include <stdint.h>

struct acc3 {
	int16_t x;
	int16_t y;
	int16_t z;
};

typedef struct acc3 acc3_t;

// Initialize accelerometer
void AccInit(void);

// Obtain a reading
void AccRead(acc3_t * reading);
