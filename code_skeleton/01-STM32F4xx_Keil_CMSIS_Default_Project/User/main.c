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

// Initialize Timer 4 for interrupts
void TMR4_Init_ISR(void) {
	// Setup the nested vector interrupt controller
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_BaseStruct;
	/* Enable the TIM4 gloabal Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
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
	/* TIM Interrupt enable */
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
	/* Start count on TIM4 */
	TIM_Cmd(TIM4, ENABLE);
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

// This is triggered when the counter overflows
void TIM4_IRQHandler(void) {
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) {
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
		pwm_update_channels();
		pwm_driver_update();
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
	TMR4_Init_ISR(); // initialize timer

	while (1) {
		volatile uint32_t buttonRegister = GPIOA->IDR;
		if(buttonRegister & 0x1) {
			pwm_driver_set(0, 100);
			pwm_driver_set(1, 100);
			pwm_driver_set(2, 100);
			pwm_driver_set(3, 100);
		}
	}
}
