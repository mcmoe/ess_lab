#include "pwm_driver.h"

// Encapsulate the state of the PWM driver
struct pwm_state {
	uint32_t counter;
	uint32_t ch0_compare;
	uint32_t ch1_compare;
	uint32_t ch2_compare;
	uint32_t ch3_compare;
	LED_t * ch0;
	LED_t * ch1;
	LED_t * ch2;
	LED_t * ch3;
};

// We declare the state as static so it only has file scope and cannot be accessed externally
// this makes the assumption that we only have one pwm_driver instance in the system
static struct pwm_state state;

void pwm_driver_init(LED_t * ch0, LED_t * ch1, LED_t * ch2, LED_t * ch3) {
	// start counter at zero
	state.counter = 0;

	// set all channels to off (i.e. compare value of 0)
	state.ch0_compare = 0;
	state.ch1_compare = 0;
	state.ch2_compare = 0;
	state.ch3_compare = 0;
	
	// store the pointers of each led driver
	state.ch0 = ch0;
	state.ch1 = ch1;
	state.ch2 = ch2;
	state.ch3 = ch3;
}

void pwm_driver_set(uint8_t channel, uint8_t value) {

	// bounds check
	if (channel > 3) {
		return;
	}

	// update the compare register depending on which channel is selected
	if (channel == 0) {
		state.ch0_compare = value;
	} else if (channel == 1) {
		state.ch1_compare = value;
	} else if (channel == 2) {
		state.ch2_compare = value;
	} else if (channel == 3) {
		state.ch3_compare = value;
	}
}

void pwm_driver_update(void) {
	
	// Update ch0: if compare value is greater than counter, turn on, else turn off
	if (state.ch0_compare > state.counter) {
		led_on(state.ch0);
	} else {
		led_off(state.ch0);
	}
	// Update ch1
	if (state.ch1_compare > state.counter) {
		led_on(state.ch1);
	} else {
		led_off(state.ch1);
	}
	// Update ch2
	if (state.ch2_compare > state.counter) {
		led_on(state.ch2);
	} else {
		led_off(state.ch2);
	}

	// Update ch3
		if (state.ch3_compare > state.counter) {
		led_on(state.ch3);
	} else {
		led_off(state.ch3);
	}

	// Update counter value
	if (state.counter++ > PWM_MAX) {
		state.counter = 0;
	}
}
