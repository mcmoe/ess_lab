#include <stdint.h>
#include "pwm_driver.h"

static uint32_t ch0 = 0;
static uint32_t ch1 = 1;
static uint32_t ch2 = 2;
static uint32_t ch3 = 3;
volatile static uint32_t currentChannel = 0;

static uint32_t ch0Brightness = 0;
static uint32_t ch1Brightness = 0;
static uint32_t ch2Brightness = 0;
static uint32_t ch3Brightness = 0;
static int32_t change = 1;
volatile static int8_t order = 1;

void pwm_updater_reverse() {
	order = order == 1 ? -1 : 1;
}

uint32_t * change_and_get(uint32_t * brightness, int32_t * change) {
	*brightness = (*brightness + *change) % 10001;
	return brightness;
}

void update_channel(volatile uint32_t * currentChannel, uint32_t * targetChannel, uint32_t * brightness, int32_t * change) {
	if (*currentChannel == *targetChannel) {
		pwm_driver_set(*targetChannel, *change_and_get(brightness, change) / 100);

		if(*brightness == 10000) {
			 *change = -1;
		} else if (*brightness == 0) {
			*currentChannel = (*currentChannel + order) % 4;
			*change = 1;
		}
	}
}

void pwm_update_channels() {
	update_channel(&currentChannel, &ch0, &ch0Brightness, &change);
	update_channel(&currentChannel, &ch1, &ch1Brightness, &change);
	update_channel(&currentChannel, &ch2, &ch2Brightness, &change);
	update_channel(&currentChannel, &ch3, &ch3Brightness, &change);
}
