#include "encoder.h"

#define TIMx TIM2
#define TIM_GPIO GPIOA

typedef enum {
	FORWARD, BACKWARD
} Direction;
volatile uint8_t capture_is_first = 1, capture_is_ready = 0;
volatile Direction captured_direction = FORWARD;

void init_encoder(void) {
	GPIO_InitTypeDef encoder;

	/* Каналы 1 и 2 таймера TIM3 - на вход, подтянуть к питанию */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA, ENABLE);
	encoder.GPIO_Mode = GPIO_Mode_IPU;
	encoder.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_Init(TIM_GPIO, &encoder);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	/* Разрешаем счёт в обе стороны, период ставим 4 */
	TIM_TimeBaseInitTypeDef timer;
	TIM_TimeBaseStructInit(&timer);
	//timer.TIM_ClockDivision = TIM_CKD_DIV1;
	timer.TIM_Prescaler = 0;
	timer.TIM_Period = 256;
	timer.TIM_CounterMode = TIM_CounterMode_Down | TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIMx, &timer);

	/* Считать будем все переходы лог. уровня с обоих каналов */
	//TIM_EncoderInterfaceConfig(TIMx, TIM_EncoderMode_TI12, TIM_ICPolarity_BothEdge, TIM_ICPolarity_BothEdge);
	TIM_EncoderInterfaceConfig(TIMx, TIM_EncoderMode_TI1, TIM_ICPolarity_Rising,
			TIM_ICPolarity_Rising);
	TIM_ITConfig(TIMx, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIMx, ENABLE);

	//NVIC_EnableIRQ(TIM2_IRQn);
}

void TIM2_IRQHandler(void) {
	if (TIM_GetITStatus(TIMx, TIM_IT_Update) != RESET) {
		TIM_ClearITPendingBit(TIMx, TIM_IT_Update);

		if (!capture_is_first)
			capture_is_ready = 1;

		capture_is_first = 0;
		/* В бите TIM_CR1_DIR регистра TIM3_CR1 хранится
		 направление вращения энкодера, запоминаем его. */
		captured_direction = (TIMx->CR1 & TIM_CR1_DIR ? FORWARD : BACKWARD);
	}
}
