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
#include "stm32f4xx_tim.h"
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

// ======= Timer functions ========

// Initialize Timer 4
void TMR4_Init(void) {
	TIM_TimeBaseInitTypeDef TIM_BaseStruct;

	/* Enable clock for TIM4 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

	// timer_tick_frequency = 84000000 / (0 + 1) = 84000000
	TIM_BaseStruct.TIM_Prescaler = 0;

	/* Count up */
	TIM_BaseStruct.TIM_CounterMode = TIM_CounterMode_Up;

	/*
	Set timer period when it must reset
	First you have to know max value for timer
	In our case it is 16bit = 65535
	Frequency = timer_tick_frequency / (TIM_Period + 1)
	If you get TIM_Period larger than max timer value (in our case 65535),
	you have to choose larger prescaler and slow down timer tick frequency
	*/
	TIM_BaseStruct.TIM_Period = 8400; // I've put a value of 8.4K so that 84mHz/ 8.4K = 10kHz
	TIM_BaseStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_BaseStruct.TIM_RepetitionCounter = 0;

  /* Initialize TIM4 */
	TIM_TimeBaseInit(TIM4, &TIM_BaseStruct);

  /* Start count on TIM4 */
	TIM_Cmd(TIM4, ENABLE);
}

// Loops until the timer has expired
void TMR4_WaitForExpiry(void) {
	// Check the flag. When the timer is expired, the flag is SET.
	while(TIM_GetFlagStatus(TIM4, TIM_FLAG_Update) == RESET)
	{
	}
	// Reset flag for next expiry
	TIM_ClearFlag(TIM4, TIM_IT_Update);
}

// ================================

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
	TMR4_Init(); // initialize timer

	while (1) {
		TMR4_WaitForExpiry();
		pwm_update_channels();
		pwm_driver_update();
	}
}
