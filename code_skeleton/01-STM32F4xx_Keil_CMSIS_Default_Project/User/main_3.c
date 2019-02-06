/**
 *	STM32F4 Discovery Skeleton Project
 *
 *	@ide		Keil uVision 5
 *	@conf		PLL parameters are set in "Options for Target" -> "C/C++" -> "Defines"
 *	@packs		STM32F4xx Keil packs version 2.2.0 or greater required
 *	@stdperiph	STM32F4xx Standard peripheral drivers version 1.5.0 or greater required
 */
/* Include core modules */
#include "stm32f4xx.h"
/* Include helper library */
#include "ess_helper.h"

#include "led_driver.h"

volatile uint32_t * ledPort_3 = (uint32_t*) 0x40020C14;

// delay by msec provided as argument
void delay_msec_3(uint32_t delay);

// return the hexadecimal representation of the pin flag
uint32_t pin_const_3(uint32_t pin);

static uint32_t led_green_pin;

// Initialize the green LED
void led_green_init(uint32_t pin);

// Turn the green LED on
void led_green_on(void);

// Turn the green LED off
void led_green_off(void);

// green is 0x1000
void led_green_init(uint32_t pin) {
	led_green_pin = pin;
}

void led_green_off(void) {
	//*ledPort = ~0x1000 & (*ledPort);
	*ledPort_3 = ~pin_const_3(led_green_pin) & (*ledPort_3);
}

void led_green_on(void) {
	//*ledPort = 0x1000 | (*ledPort);
	*ledPort_3 = pin_const_3(led_green_pin) | (*ledPort_3);
}

void delay_msec_3(uint32_t delay) {
		uint32_t i = 0;
		while(i < delay * 42000) {
			i++;
		}
}

uint32_t pin_const_3(uint32_t pin) {
	return 0x1 << pin;
}

int mainx(void) {
	/* Initialize system */
	SystemInit();
	/* Initialize peripherals on board */
	ess_helper_init();
	// Main loop 
	
//	*(uint32_t*) 0x40020C14 = 0x0000;  // No LEDs
//	*(uint32_t*) 0x40020C14 = 0x1000; // left green
//	*(uint32_t*) 0x40020C14 = 0x2000;	// top orange
//	*(uint32_t*) 0x40020C14 = 0x4000;	// right red
//	  *(uint32_t*) 0x40020C14 = 0x8000; // bottom blue
//	*(uint32_t*) 0x40020C14 = 0xF000; // all LEDs

	led_green_init(12);
	//*ledPort = 0x4000 | (*ledPort);

	while (1) {
		delay_msec_3(1000);
		led_green_on();
		delay_msec_3(10000);
		led_green_off();
	}
}


/*

Hex 	Binary 	Decimal
0x0 	0000 			0
0x1 	0001 			1		
0x2 	0010 			2	
0x4 	0100 			4	
0x8 	1000 			8
0xF 	1111		 	15

Hex 	Binary 	Decimal
0x0 	0000 			0 			
0x1 	0001 			1 	<<		
0x2 	0010 			2 	<<	
0x3 	0011 			3 			
0x4 	0100 			4   <<			
0x5 	0101 			5 			
0x6 	0110 			6 			
0x7 	0111 			7 			
0x8 	1000 			8   <<
0x9 	1001		 	9
0xA 	1010		 	10
0xB 	1011 			11
0xC 	1100		 	12
0xD 	1101 			13
0xE 	1110		 	14
0xF 	1111		 	15  <<
*/
