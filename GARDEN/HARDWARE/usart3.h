#ifndef _USART3_H_
#define _USART3_H_

#include "stm32f10x.h"
#include "stdarg.h"
#include "stdio.h"
#include "string.h"

extern uint8_t USART3_RXBUF[20];
extern uint8_t USART3_TXBUF[20];
extern uint8_t USART3_RXCUNT;
extern uint8_t USART3_RXOVER;

void USART3_Config(void);
void USART3_SendString(uint8_t *str);
void u3_printf(char* fmt,...);

extern void DelayMs(uint32_t nTime);

#endif
