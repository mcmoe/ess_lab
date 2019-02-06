#include <stdio.h>
#include "minunit.h"
#include "led_driver_test_wrapper.h"

#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYLW  "\x1B[33m"
#define KEND  "\033[0m"

int tests_run = 0;

int foo = 7;

static char * test_foo() {
	mu_assert("ERROR: foo !- 7", foo == 7);
	return 0;
}

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
	mu_assert("ERROR: led pin was not set", led.pin != 0);

	uint32_t finalPort = *led.port;
	mu_assert("ERROR: port register had some bits set", (finalPort & ~initialPort) == 0);
	mu_assert("ERROR: port register changed by more than one bit", finalPort == initialPort || countZeros(finalPort) == initialZeros + 1);
    mu_assert("ERROR: led port was not unset", ((finalPort >> 2) & 0x1) == 0);
	return 0;
}

static char * test_led_on() {
	LED_t led = {0};
 	hal_led_init(&led, 5);
	uint32_t initialPort = *led.port;
	uint32_t initialZeros = countZeros(initialPort);
	hal_led_on(&led);
	uint32_t finalPort = *led.port;

	mu_assert("ERROR: no port register bit set", (finalPort & ~initialPort) != 0);
	mu_assert("ERROR: port register changed by more than one bit", countZeros(finalPort) == initialZeros - 1);
    mu_assert("ERROR: led port was not set", ((finalPort >> 5) & 0x1) == 1);
	return 0;
}

static char * all_tests() {
	mu_run_test(test_foo);
	mu_run_test(test_led_init);
	mu_run_test(test_led_on);
	return 0;
}

int testMain(int argc, char **argv) {
	char *result = all_tests();
	if (result != 0) {
		printf("%s%s%s\n", KRED, result, KEND);
	} else {
		printf("%sALL TESTS PASSED%s\n", KGRN, KEND);
	}
	printf("%sTests run: %d%s\n", KYLW, tests_run, KEND);

	return result != 0;
}
