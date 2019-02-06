#include <stdint.h>
#include "led_driver_test_wrapper.h"
#include "led_driver.h"

static uint32_t mock_address = 0xFFFFFFFF;
static uint32_t* mock_port = &mock_address;

uint32_t* get_hal_port(void) {
	return mock_port;
}

void hal_led_init(LED_t * led, uint32_t pin) {
	led_init(led, mock_port, pin);
	//led_on(led); // check that if bit is not unset or was accidentally set - test will fail
	//LED_t x = { mock_port, 3};
	//led_toggle(&x); // check that if another bit is changed - test will fail
}

void hal_led_on(LED_t * led) {
	led_on(led);
}
void hal_led_off(LED_t * led) {
	led_off(led);
}
void hal_led_toggle(LED_t * led) {	
	led_toggle(led);
}

uint8_t hal_led_read(LED_t * led) {
	return led_read(led);
}
