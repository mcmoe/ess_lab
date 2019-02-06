#include <stdio.h>
#include "minunit.h"
#include "led_driver_test_wrapper.h"
#include "pwm_driver.h"
#include "pwm_updater.h"

#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYLW  "\x1B[33m"
#define KEND  "\033[0m"

int tests_run = 0;

static uint32_t countZeros(uint32_t i) {
uint32_t count = 0;
  while(i > 0) {
	  count += ~i & 0x1;
	  i >>= 0x1;
  }

  return count;
}

static char * test_led_init() {
	LED_t led = {0};
	uint32_t initialPort = *get_hal_port();
	uint32_t initialZeros = countZeros(initialPort);

 	hal_led_init(&led, 2);
	mu_assert("led pin was not set", led.pin != 0);

	uint32_t finalPort = *led.port;
	mu_assert("port register had some bits set", (finalPort & ~initialPort) == 0);
	mu_assert("port register changed by more than one bit", finalPort == initialPort || countZeros(finalPort) == initialZeros + 1);
    mu_assert("led port was not unset", ((finalPort >> 2) & 0x1) == 0);
	return 0;
}

static char * test_led_on() {
	LED_t led = {0};
 	hal_led_init(&led, 5);
	uint32_t initialPort = *led.port;
	uint32_t initialZeros = countZeros(initialPort);
	hal_led_on(&led);
	uint32_t finalPort = *led.port;

	mu_assert("no port register bit set", (finalPort & ~initialPort) != 0);
	mu_assert("port register changed by more than one bit", countZeros(finalPort) == initialZeros - 1);
	mu_assert("led port was not set", ((finalPort >> 5) & 0x1) == 1);
	return 0;
}

static char * test_pwm_update() {
	LED_t led0, led1, led2, led3 = {0};
 	hal_led_init(&led0, 0);
 	hal_led_init(&led1, 1);
 	hal_led_init(&led2, 2);
 	hal_led_init(&led3, 3);

	pwm_driver_init(&led0, &led1, &led2, &led3);

	pwm_driver_update();
	
	mu_assert("led0 should be off", hal_led_read(&led0) == 0);
	mu_assert("led1 should be off", hal_led_read(&led1) == 0);
	mu_assert("led2 should be off", hal_led_read(&led2) == 0);
	mu_assert("led3 should be off", hal_led_read(&led3) == 0);

	pwm_driver_set(0, 100);
	pwm_driver_update();
	mu_assert("led0 should be on", hal_led_read(&led0) == 1);
	mu_assert("led1 should be off", hal_led_read(&led1) == 0);
	mu_assert("led2 should be off", hal_led_read(&led2) == 0);
	mu_assert("led3 should be off", hal_led_read(&led3) == 0);

	pwm_driver_set(1, 100);
	pwm_driver_update();
	mu_assert("led0 should be on", hal_led_read(&led0) == 1);
	mu_assert("led1 should be on", hal_led_read(&led1) == 1);
	mu_assert("led2 should be off", hal_led_read(&led2) == 0);
	mu_assert("led3 should be off", hal_led_read(&led3) == 0);

	pwm_driver_set(2, 100);
	pwm_driver_update();
	mu_assert("led0 should be on", hal_led_read(&led0) == 1);
	mu_assert("led1 should be on", hal_led_read(&led1) == 1);
	mu_assert("led2 should be on", hal_led_read(&led2) == 1);
	mu_assert("led3 should be off", hal_led_read(&led3) == 0);

	pwm_driver_set(3, 100);
	pwm_driver_update();
	mu_assert("led0 should be on", hal_led_read(&led0) == 1);
	mu_assert("led1 should be on", hal_led_read(&led1) == 1);
	mu_assert("led2 should be on", hal_led_read(&led2) == 1);
	mu_assert("led3 should be on", hal_led_read(&led3) == 1);

	return 0;	
}

static char * test_pwm_channel_updater() {
	LED_t led0, led1, led2, led3 = {0};
 	hal_led_init(&led0, 0);
 	hal_led_init(&led1, 1);
 	hal_led_init(&led2, 2);
 	hal_led_init(&led3, 3);

	pwm_driver_init(&led0, &led1, &led2, &led3);

  uint32_t i;

	for(i = 0; i < 100; ++i) {
		pwm_update_channels();
	}
	
	pwm_driver_update();

	mu_assert("led0 should be on", hal_led_read(&led0) == 1);
	mu_assert("led1 should be off", hal_led_read(&led1) == 0);
	mu_assert("led2 should be off", hal_led_read(&led2) == 0);
	mu_assert("led3 should be off", hal_led_read(&led3) == 0);
	
	for(i = 0; i < 100; ++i) {
		pwm_update_channels();
	}
	
	pwm_driver_update();

	mu_assert("led0 should be off", hal_led_read(&led0) == 0);
	mu_assert("led1 should be off", hal_led_read(&led1) == 0);
	mu_assert("led2 should be off", hal_led_read(&led2) == 0);
	mu_assert("led3 should be off", hal_led_read(&led3) == 0);

	for(i = 0; i < 100; ++i) {
		pwm_update_channels();
	}
	
	pwm_driver_update();

	mu_assert("led0 should be off", hal_led_read(&led0) == 0);
	mu_assert("led1 should be on", hal_led_read(&led1) == 1);
	mu_assert("led2 should be off", hal_led_read(&led2) == 0);
	mu_assert("led3 should be off", hal_led_read(&led3) == 0);
	
	return 0;
}

static char * all_tests() {
	mu_run_test(test_led_init);
	mu_run_test(test_led_on);
	mu_run_test(test_pwm_update);
	mu_run_test(test_pwm_channel_updater);
	return 0;
}

int testMain(int argc, char **argv) {
	char *result = all_tests();
	if (result != 0) {
		printf("%sERROR: %s%s\n", KRED, result, KEND);
	} else {
		printf("%sALL TESTS PASSED%s\n", KGRN, KEND);
	}
	printf("%sTests run: %d%s\n", KYLW, tests_run, KEND);

	return result != 0;
}
