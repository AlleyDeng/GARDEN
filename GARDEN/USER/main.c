#include "stm32f10x.h"
#include "usart1.h"
#include "usart3.h"
#include "wifi.h"
#include "led.h"

uint8_t USART1_RXBUF[20];
uint8_t USART1_TXBUF[20];
uint8_t USART1_RXCUNT = 0;
uint8_t USART1_RXOVER = 0;

uint8_t USART3_RXBUF[20];
uint8_t USART3_TXBUF[20];
uint8_t USART3_RXCUNT = 0;
uint8_t USART3_RXOVER = 0;

uint32_t timingDelay = 0;
uint8_t str[20] = "                    ";

void DelayMs(uint32_t nTimeDelay)
{
	timingDelay = nTimeDelay;
	while (timingDelay != 0);
}

int main (void)
{
	SysTick_Config(SystemCoreClock/1000);
	USART1_Config();
	USART3_Config();
	LED_Init();
	
	printf ("USART1\r\n");
	u3_printf("AT\r\n");
	while (1) {
//		if (USART1_RXOVER == 1) {
//			USART1_RXOVER = 0;
//			USART1_SendString("USART1:\r\n");
//			USART1_SendString(USART1_RXBUF);
//			USART3_SendString(USART1_RXBUF);
//			USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
//			
//			GPIO_ResetBits(GPIOE, GPIO_Pin_5);
//			DelayMs(100);
//		} else if  (USART1_RXOVER == 0) {
//			GPIO_SetBits(GPIOE, GPIO_Pin_5);	
//		}
//		
//		if (USART3_RXOVER == 1) {
//			USART3_RXOVER = 0;
//			u3_printf("%s\r\n", USART3_RXBUF);
// 			USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
//			GPIO_ResetBits(GPIOB, GPIO_Pin_5);
//			DelayMs(100);
//		}	else if (USART3_RXOVER == 0) {
//			GPIO_SetBits(GPIOB, GPIO_Pin_5);
//		}
	}
}
