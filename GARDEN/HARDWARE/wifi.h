#ifndef _WIFI_H_
#define _WIFI_H_

#include "stm32f10x.h"
#include "usart1.h"
#include "usart3.h"

extern uint8_t USART1_RXBUF[20];
extern uint8_t USART1_RXCUNT;
extern uint8_t USART1_RXOVER;

extern uint8_t USART3_RXBUF[20];
extern uint8_t USART3_RXCUNT;
extern uint8_t USART3_RXOVER;

void WIFI_Response(uint8_t mode);
uint8_t WIFI_SendCmd(uint8_t *cmd, uint8_t *ack, uint8_t waittime);






#endif
