#ifndef __RTC_H__
#define __RTC_H__

#include "stm32f10x.h"

void rtc_init(void);
void setTime(uint32_t time);

#endif
