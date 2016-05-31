#include "utils.h"

#define SYSTICK_MAX_VALUE 16777215

static volatile STATES_t STATE = DISPLAY_TIME;

STATES_t getState(void) {
	return STATE;
}

void setState(STATES_t newState) {
	STATE = newState;
}

void delay(uint32_t time) {
	if (time > SYSTICK_MAX_VALUE)
		return;
	SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;
	SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;
	SysTick->LOAD = (24 * time);
	SysTick->VAL = 0;
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
	while (!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk))
		;
	SysTick->CTRL &= ~SysTick_CTRL_COUNTFLAG_Msk;
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}

void temperatureToDigits(uint16_t temperature, uint8_t* result) {
	for (uint32_t i = 0; i < 3; i++) {
		result[2 - i] = temperature % 10;
		temperature /= 10;
	}
}

void timeToDigits(uint8_t hours, uint8_t minutes, uint8_t* result) {
	result[1] = hours % 10;
	result[0] = (hours / 10) % 10;
	result[3] = minutes % 10;
	result[2] = (minutes / 10) % 10;
}

void cntToDigits(uint32_t cnt, uint8_t* result) {
	result[0] = 0;
	result[1] = (cnt / 100) % 10;
	result[2] = (cnt / 10) % 10;
	result[3] = cnt % 10;
}

void intensivityToDigits(uint32_t cnt, uint8_t result) {
	result = cnt/16;
}

//void light_init() {
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
//	GPIO_InitTypeDef LED;
//
//	LED.GPIO_Pin = GPIO_Pin_0;
//	LED.GPIO_Speed = GPIO_Speed_2MHz;
//	LED.GPIO_Mode = GPIO_Mode_Out_PP;
//
//	GPIO_Init(GPIOB, &LED);
//}

//void switchLight(FunctionalState state) {
//	if (state == ENABLE) {
//		GPIO_SetBits(GPIOB, GPIO_Pin_0);
//	} else {
//		GPIO_ResetBits(GPIOB, GPIO_Pin_0);
//	}
//}

void PLLInit(void) {
	RCC_DeInit();

	RCC_HSICmd(ENABLE);
	while (!RCC_GetFlagStatus(RCC_FLAG_HSIRDY))
		;

	RCC_PLLConfig(RCC_PLLSource_HSI_Div2, RCC_PLLMul_6);
	RCC_PLLCmd(ENABLE);
	while (!RCC_GetFlagStatus(RCC_FLAG_PLLRDY))
		;

	FLASH_SetLatency(FLASH_Latency_1);
	FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

	RCC_HCLKConfig(RCC_SYSCLK_Div1);
	RCC_PCLK1Config(RCC_HCLK_Div1);
	RCC_PCLK2Config(RCC_HCLK_Div1);
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
	while (RCC_GetSYSCLKSource() != 0x08)
		;
}

uint8_t getInteger(uint8_t f_part, uint8_t s_part) {
	return f_part * 10 + s_part;
}

#define SECONDS_IN_HOUR 3600
#define SECONDS_IN_MINUTES 60
uint32_t getTime(uint8_t hour, uint8_t minutes, uint8_t seconds) {
	return hour * SECONDS_IN_HOUR + minutes * SECONDS_IN_MINUTES + seconds;
}
