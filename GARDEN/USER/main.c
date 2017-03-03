#include "stm32f10x.h"
#include "usart3.h"
#include "led.h"

uint8_t USART1_RXBUF[20];
uint8_t USART1_RXCUNT = 0;
uint8_t USART1_RXOVER = 0;
uint8_t USART3_RXBUF[20];
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
	USART3_Config();
	LED_Init();
	
//	USART_SendData(USART2, 'x');
	USART3_SendString("Start\r\n");
	while (1) {
		GPIO_SetBits(GPIOE, GPIO_Pin_5);
		DelayMs(100);
		GPIO_ResetBits(GPIOE, GPIO_Pin_5);
		DelayMs(100);
		if (USART3_RXOVER == 1) {
			USART3_RXOVER = 0;
			USART3_SendString(USART3_RXBUF);
			USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
			GPIO_ResetBits(GPIOB, GPIO_Pin_5);
			DelayMs(100);			
			GPIO_SetBits(GPIOB, GPIO_Pin_5);
			DelayMs(100);
		}
	}
}
