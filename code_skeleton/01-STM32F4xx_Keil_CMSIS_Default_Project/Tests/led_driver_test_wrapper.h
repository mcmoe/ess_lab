#ifndef LED_DRIVER_TEST_WRAPPER_H
#define LED_DRIVER_TEST_WRAPPER_H

#include <stdint.h>
#include "led_driver.h"

uint32_t* get_hal_port(void);
void hal_led_init(LED_t * led, uint32_t pin);
void hal_led_on(LED_t * led);
void hal_led_off(LED_t * led);
void hal_led_toggle(LED_t * led);
uint8_t hal_led_read(LED_t * led);

#endif
