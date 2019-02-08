/**
* A STM32FX hardware timer wrapper to facilitate creation and subscription
*/

#ifndef ESS_TIMER_H
#define ESS_TIMER_H

#include "stm32f4xx_tim.h"

/**
* Initialize the TIM3 ISR timer along with a callback to trigger whenever timer expires
*/
void timer3_init(void (*callback) (void));

/**
* Initialize the TIM4 ISR timer along with a callback to trigger whenever timer expires
*/
void timer4_init(void (*callback) (void));

#endif
