#ifndef _USART_H_
#define _USART_H_

#include "stm32f10x.h"

extern uint8_t USART1_RXBUF[20];
extern uint8_t USART1_RXCUNT;
extern uint8_t USART1_RXOVER;

void USART1_Config(void);
void USART1_SendString(uint8_t *str);

extern void DelayMs(uint32_t nTime);

#endif
