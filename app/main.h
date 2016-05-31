#ifndef __MAIN_H__
#define __MAIN_H__

#include "stm32f10x.h"

#include "utils.h"

#include "ds18B20.h"
#include "max7219.h"
#include "esp8266.h"
#include "rtc.h"
#include "encoder.h"
#include "button.h"
#include "leds.h"
#include "pwm.h"

#include "stm32f10x_it.h"

void init_mcu(void);

#endif /* __MAIN_H_ */
