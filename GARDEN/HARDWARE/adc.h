#ifndef _ADC_H_
#define _ADC_H_

#include "sys.h"
#include "delay.h"

void ADC_Config(void);

u16 Get_Adc(u8 ch);
u16 Get_Adc_Average(u8 ch, u8 times);


#endif
