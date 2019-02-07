/**
* A timer based on Timer4
*/

#ifndef ESS_TIMER_H
#define ESS_TIMER_H

#include "stm32f4xx_tim.h"

/**
* Initialize the TIM4 ISR timer along with a callback to trigger whenever timer expires
*/
void timer_init(void (*callback) (void));

#endif
