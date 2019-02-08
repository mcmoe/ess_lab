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
#include "ess_timer.h"

#include "led_driver.h"
#include "pwm_driver.h"
#include "pwm_updater.h"
#include "spi_driver.h"
#include "acc_reader.h"
#include "display_driver.h"

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

static uint8_t button_pressed = 0;
void respond_to_button() {
 volatile uint32_t buttonRegister = GPIOA->IDR;
	if(buttonRegister & 0x1) {
		button_pressed = 1;
		pwm_updater_reverse();
		delay_msec(1000);
	}
}

void update_leds(void) {
	if(button_pressed == 1) {
		pwm_update_channels();
	}
	pwm_driver_update();
}

void (*timer_function) (void) = &update_leds;
static LED_t greenLed, orangeLed, redLed, blueLed = { 0 };
void init() {
	/* Initialize system */
	SystemInit();
	/* Initialize peripherals on board */
	ess_helper_init();
  /* Initialize use case */

	led_init(&greenLed, PORTD, greenPin);
	led_init(&orangeLed, PORTD, orangePin);
	led_init(&redLed, PORTD, redPin);
	led_init(&blueLed, PORTD, bluePin);	

	pwm_driver_init(&greenLed, &orangeLed, &redLed, &blueLed);
	timer_init(timer_function); // initialize timer including the desired callback
	AccInit();
}

static acc3_t reading = { 0 };
void sense_tilt_and_display() {
	AccRead(&reading);
	display_tilt(reading.x, reading.y);
}

int main(void) {
	init();
	while (1) {
		respond_to_button();
		if(button_pressed == 0) {
			sense_tilt_and_display();
			delay_msec(10);
		}
	}
}
