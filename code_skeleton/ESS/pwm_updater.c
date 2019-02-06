#include <stdint.h>
#include "pwm_driver.h"

static uint32_t ch0 = 0;
static uint32_t ch1 = 1;
static uint32_t ch2 = 2;
static uint32_t ch3 = 3;
static uint32_t * currentChannel = &ch0;

static uint32_t ch0Brightness = 0;
static uint32_t ch1Brightness = 0;
static uint32_t ch2Brightness = 0;
static uint32_t ch3Brightness = 0;
static int32_t change = 1;

uint32_t * change_and_get(uint32_t * brightness, int32_t * change) {
	*brightness = (*brightness + *change) % 101;
	return brightness;
}

void update_channel(uint32_t ** currentChannel, uint32_t * targetChannel, uint32_t * brightness, int32_t * change, uint32_t * nextChannel) {
	if (**currentChannel == *targetChannel) {
		pwm_driver_set(*targetChannel, *change_and_get(brightness, change));

		if(*brightness == 100) {
			 *change = -1;
		} else if (*brightness == 0) {
			*currentChannel = nextChannel;
			*change = 1;
		}
	}
}

void pwm_update_channels() {
	update_channel(&currentChannel, &ch0, &ch0Brightness, &change, &ch1);		
	update_channel(&currentChannel, &ch1, &ch1Brightness, &change, &ch2);
	update_channel(&currentChannel, &ch2, &ch2Brightness, &change, &ch3);
	update_channel(&currentChannel, &ch3, &ch3Brightness, &change, &ch0);
}
