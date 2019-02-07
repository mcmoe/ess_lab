#include "stm32f4xx_tim.h"

static void (*timer_function) (void);

// Initialize Timer 4 for interrupts
void timer_init(void (*callback) (void)) {
	timer_function = callback;

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

// This is triggered when the counter overflows
void TIM4_IRQHandler(void) {
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) {
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
		timer_function();
	}
}
