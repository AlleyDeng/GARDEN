#ifndef _RTC_H_
#define _RTC_H_

#include "stm32f10x.h"
#include "lcd.h"
#include <stdio.h>

#define HH 10
#define MM 59
#define SS 50

void RTC_Init(void);
void TimeDisplay(uint32_t timeVar);

extern void DelayMs(uint32_t nTime) ;


#endif
