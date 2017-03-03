/**
  ******************************************************************************
  * @file    GPIO/IOToggle/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and peripherals
  *          interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h" 


 
void NMI_Handler(void)
{
}
 
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}
 
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

 
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}
 
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}
 
void SVC_Handler(void)
{
}
 
void DebugMon_Handler(void)
{
}
 
void PendSV_Handler(void)
{
}
 
void SysTick_Handler(void)
{
	extern uint32_t timingDelay;
	timingDelay --;
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/
void USART1_IRQHandler(void)
{
	uint8_t temp;
	extern uint8_t USART1_RXCUNT;
	extern uint8_t USART1_RXOVER;
	extern uint8_t USART1_RXBUF[20];
	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
		temp = USART_ReceiveData(USART1);
		if (temp == '\r' || temp == '#' || USART1_RXCUNT == 20) {
			USART1_RXBUF[USART1_RXCUNT] = '\r';
			USART1_RXCUNT = 0;
			USART1_RXOVER = 1;
			USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);
		}	else {
			USART1_RXBUF[USART1_RXCUNT++] = temp;
		}
	}
}

void USART3_IRQHandler(void)
{
	uint8_t temp;
	extern uint8_t USART3_RXCUNT;
	extern uint8_t USART3_RXOVER;
	extern uint8_t USART3_RXBUF[20];
	if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) {
		USART_ClearITPendingBit(USART3, USART_IT_RXNE);
		temp = USART_ReceiveData(USART3);
		if (temp == '\r' || temp == '#' || USART3_RXCUNT == 20) {
			USART3_RXBUF[USART3_RXCUNT] = '\r';
			USART3_RXCUNT = 0;
			USART3_RXOVER = 1;
			USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);
		}	else {
			USART3_RXBUF[USART3_RXCUNT++] = temp;
		}
	}
}

