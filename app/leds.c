#include "leds.h"

GPIO_InitTypeDef LEDS;

void init_leds() {
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
		GPIO_InitTypeDef LED;

		LED.GPIO_Pin = GPIO_Pin_0;
		LED.GPIO_Speed = GPIO_Speed_2MHz;
		LED.GPIO_Mode = GPIO_Mode_Out_PP;

		GPIO_Init(GPIOB, &LED);
}

void switchLight(FunctionalState state) {
	if (state == ENABLE) {
		GPIO_SetBits(GPIOB, GPIO_Pin_0);
	} else {
		GPIO_ResetBits(GPIOB, GPIO_Pin_0);
	}
}
