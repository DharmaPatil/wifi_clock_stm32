#include "stm32f10x_it.h"

void EXTI3_IRQHandler(void) {
	switch (getState()) {
	case DISPLAY_TIME:
		setState(DISPLAY_TEMPERATURE);
		break;
	case DISPLAY_TEMPERATURE:
		if (0 == 0)
			setState(SET_HOURS);
		else
			setState(DISPLAY_TIME);
		break;
	case SET_HOURS:
		setState(SET_MINUTES);
		break;
	case SET_MINUTES:
	default:
		setState(DISPLAY_TIME);
		break;
	}
	EXTI->PR |= EXTI_PR_PR3;
}

void HardFault_Handler(void) {
	while (1) {
	}
}

void MemManage_Handler(void) {
	while (1) {
	}
}

void BusFault_Handler(void) {
	while (1) {
	}
}

void UsageFault_Handler(void) {
	while (1) {
	}
}
