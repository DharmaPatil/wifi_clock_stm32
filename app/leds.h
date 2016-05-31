#ifndef __LEDS_H__
#define __LEDS_H__

#include "stm32f10x.h"

void init_leds(void);
void switchLight(FunctionalState state);

#endif /* __LEDS_H__ */
