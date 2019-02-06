#include "led_driver.h"


//----------------------
// Private Functions
//----------------------
uint32_t pin_const(uint32_t pin) {
	return 0x1 << pin;
}

//----------------------
// Public Functions
//----------------------

// Initialize an LED, start in off-state
void led_init(LED_t * led, volatile uint32_t * port, uint32_t pin) {
	led->port = port;
	led->pin = pin;
	led_off(led);
}

// Turn the LED on
void led_on(LED_t * led) {
	*led->port = pin_const(led->pin) | (*led->port);
}

// Turn the LED off
void led_off(LED_t * led) {
	*led->port = ~pin_const(led->pin) & (*led->port);
}

// Toggle LED from off to on and vice-versa
void led_toggle(LED_t * led) {
	*led->port = pin_const(led->pin) ^ (*led->port);
}

// Returns whether LED is set (1) or clear (0)
uint8_t led_read(LED_t * led) {
	return (pin_const(led->pin) & (*led->port)) > 0;
}

