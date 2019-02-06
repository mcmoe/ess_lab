/**
 *	STM32F4 Discovery Skeleton Project
 *
 *	@ide		Keil uVision 5
 *	@conf		PLL parameters are set in "Options for Target" -> "C/C++" -> "Defines"
 *	@packs		STM32F4xx Keil packs version 2.2.0 or greater required
 *	@stdperiph	STM32F4xx Standard peripheral drivers version 1.5.0 or greater required
 */

#include <stdio.h>

/* Include core modules */
#include "stm32f4xx.h"
/* Include helper library */
#include "ess_helper.h"

#include "led_driver.h"
#include "pwm_driver.h"

// Use a define for the address of the PORTD ouput register
#define PORTD ((volatile uint32_t*) 0x40020C14)

const uint32_t greenPin = 12;
const uint32_t orangePin = 13;
const uint32_t redPin = 14;
const uint32_t bluePin = 15;

int itm_debug(int c){
  return(ITM_SendChar(c));
}

int fputc(int ch, FILE *f) {
	/* Do your stuff here */
	/* Send your custom byte */
	
	
	/* If everything is OK, you have to return character written */
	return itm_debug(ch);
	/* If character is not correct, you can return EOF (-1) to stop writing */
	//return -1;
}

// delay by usec provided as argument
void delay_usec(uint32_t delay);
// delay by msec provided as argument
void delay_msec(uint32_t delay);

void delay_usec(uint32_t delay) {
		uint32_t i = 0;
		while(i < delay * 42) {
			i++;
		}
}

void delay_msec(uint32_t delay) {
		delay_usec(delay * 1000);
}

void circle_through_lights_on_button_press(uint32_t* choice, LED_t * led) {
	volatile uint32_t buttonRegister = GPIOA->IDR;

	if(buttonRegister & 0x1) {
			led_off(led);
			*choice = (*choice + 1) % 4;
			led_init(led, PORTD, 12 + *choice);
			led_on(led);
			//led_toggle(led);
		}
		delay_msec(1000);
}

void pwm_main(LED_t * led, uint32_t * on_time, uint32_t * off_time, uint32_t a_second, uint32_t * i, int32_t * change) {
	led_on(led);
	delay_msec(*on_time);
	led_off(led);
	delay_msec(*off_time);

	//volatile uint32_t buttonRegister = GPIOA->IDR;
	if(/*buttonRegister & 0x1 && */(*i)++ == a_second) {
		if(*on_time == 10) {
			*change = -1;
		} else if(*on_time == 0) {
			*change = 1;
		}
		*on_time = (*on_time + *change);
		*off_time = (10 - (*on_time));
		*i = 0;
	}
}

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

int main(void) {
	/* Initialize system */
	SystemInit();
	/* Initialize peripherals on board */
	ess_helper_init();
	// Main loop 

	LED_t greenLed = { 0 };
	led_init(&greenLed, PORTD, greenPin);
	
	LED_t orangeLed = { 0 };
	led_init(&orangeLed, PORTD, orangePin);

	LED_t redLed = { 0 };
	led_init(&redLed, PORTD, redPin);

	LED_t blueLed = { 0 };
	led_init(&blueLed, PORTD, bluePin);	

	uint32_t greenChannel = 0;
	uint32_t orangeChannel = 1;
	uint32_t redChannel = 2;
	uint32_t blueChannel = 3;
	uint32_t * currentChannel = &greenChannel;
	
	uint32_t greenBrightness = 0;
	uint32_t orangeBrightness = 0;
	uint32_t redBrightness = 0;
	uint32_t blueBrightness = 0;
	int32_t change = 1;
	
	pwm_driver_init(&greenLed, &orangeLed, &redLed, &blueLed);	
	const uint32_t period = 100;

	while (1) {
		delay_usec(100);

		update_channel(&currentChannel, &greenChannel, &greenBrightness, &change, &orangeChannel);		
		update_channel(&currentChannel, &orangeChannel, &orangeBrightness, &change, &redChannel);
		update_channel(&currentChannel, &redChannel, &redBrightness, &change, &blueChannel);
		update_channel(&currentChannel, &blueChannel, &blueBrightness, &change, &greenChannel);

		uint32_t i = 0;
		for (;i <= period * 50; i++) {
			pwm_driver_update();
		}	
	}
}
