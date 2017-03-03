#ifndef _USART_H_
#define _USART_H_

#include "stm32f10x.h"

extern uint8_t USART3_RXBUF[20];
extern uint8_t USART3_RXCUNT;
extern uint8_t USART3_RXOVER;

void USART3_Config(void);
void USART3_SendString(uint8_t *str);

extern void DelayMs(uint32_t nTime);

#endif
