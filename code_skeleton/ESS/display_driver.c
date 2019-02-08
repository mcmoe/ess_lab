#include "display_driver.h"
#include "pwm_driver.h"

static uint8_t leftCh = 0;
static uint8_t topCh = 1;
static uint8_t rightCh = 2;
static uint8_t bottomCh = 3;

void display_init(LED_t * left, LED_t * top, LED_t * right, LED_t * bottom) {
	pwm_driver_init(left, top, right, bottom);
}

static uint8_t positive_led_value(int8_t tilt) {
	return tilt > 45 ? 100 : tilt > 2 ? tilt * 2 : 0;
}

static uint8_t negative_led_value(int8_t tilt) {
	return tilt < -45 ? 100 : tilt < -2 ? tilt * -2 : 0;
}

/*
* Display a two-dimensional tilt
* x_tilt varies from -90 deg to +90deg
* y_tilt varies from -90 deg to +90deg
*/
void display_tilt(int8_t x_tilt, int8_t y_tilt) {
	pwm_driver_set(rightCh,  positive_led_value(x_tilt));
	pwm_driver_set(leftCh,   negative_led_value(x_tilt));
	pwm_driver_set(topCh,    positive_led_value(y_tilt));
	pwm_driver_set(bottomCh, negative_led_value(y_tilt));
}
