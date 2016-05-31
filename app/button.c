#include "button.h"

GPIO_InitTypeDef BUTTON;

void init_button(void) {
	BUTTON.GPIO_Pin = GPIO_Pin_3;
	BUTTON.GPIO_Speed = GPIO_Speed_2MHz;
	BUTTON.GPIO_Mode = GPIO_Mode_IPD;

	GPIO_Init(GPIOA, &BUTTON);

	// + Тактирование
	NVIC_EnableIRQ(EXTI3_IRQn);
	EXTI->IMR |= EXTI_IMR_MR3;
	EXTI->RTSR |= EXTI_RTSR_TR3;
}
