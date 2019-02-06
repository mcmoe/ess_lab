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
#include "pwm_updater.h"

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

int main(void) {
	/* Initialize system */
	SystemInit();
	/* Initialize peripherals on board */
	ess_helper_init();
	// Main loop 

	LED_t greenLed, orangeLed, redLed, blueLed = { 0 };
	led_init(&greenLed, PORTD, greenPin);
	led_init(&orangeLed, PORTD, orangePin);
	led_init(&redLed, PORTD, redPin);
	led_init(&blueLed, PORTD, bluePin);	

	pwm_driver_init(&greenLed, &orangeLed, &redLed, &blueLed);	
	const uint32_t period = 100;

	while (1) {
		delay_usec(100);
		pwm_update_channels();

		uint32_t i = 0;
		for (;i <= period * 50; i++) {
			pwm_driver_update();
		}	
	}
}
