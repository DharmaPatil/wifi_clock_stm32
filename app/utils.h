#ifndef __UTILS_H__
#define __UTILS_H__

#include "stm32f10x.h"

void PLLInit(void);

typedef enum {
	DISPLAY_TIME,
	DISPLAY_TEMPERATURE,
	SET_HOURS,
	SET_MINUTES
} STATES_t;

STATES_t getState(void);
void setState(STATES_t newState);
void delay(uint32_t time);
void temperatureToDigits(uint16_t temperature, uint8_t* result);
void timeToDigits(uint8_t hours, uint8_t minutes, uint8_t* result);
void cntToDigits(uint32_t cnt, uint8_t* result);
void intensivityToDigits(uint32_t cnt, uint8_t result);
uint8_t getInteger(uint8_t f_part, uint8_t s_part);
uint32_t getTime(uint8_t hour, uint8_t minutes, uint8_t seconds);

//void switchLight(FunctionalState state);
//void light_init();

#endif /* __UTILS_H__ */
