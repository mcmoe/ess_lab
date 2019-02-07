#include <stdint.h>
#include "led_driver.h"

#ifndef DISPLAY_DRIVER_H 
#define DISPLAY_DRIVER_H

/**
• R6.1 When the board is perfectly horizontal, all LEDs should be off
• R6.2 When the board is tilted, it should indicate the degree of tilt by controlling the brightness of the LEDs
• R6.3 When the board is tilted beyond 45 degrees on any axis, that respective LED should have a brightness of 100%
*/

// Initialize the tilt display using PWM driver
void display_init(LED_t * left, LED_t * top, LED_t * right, LED_t * bottom);

// Display a two-dimensional tilt.
// x_tilt varies from -90 deg to +90deg. y_tilt varies from -90 deg to +90deg.
void display_tilt(int8_t x_tilt, int8_t y_tilt);

#endif
