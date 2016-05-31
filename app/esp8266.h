#ifndef __ESP8266_H__
#define __ESP8266_H__

#include "stm32f10x.h"



static volatile uint8_t ready = 0;
static volatile uint32_t odd = 0;

void esp8266_init(void);
void transmit(uint8_t data);
void cleanBuffer(void);

#endif /* __ESP8266_H__ */
