#include "delay.h"
#include "usart3.h"
#include "stdarg.h"	 	 
#include "stdio.h"	 	 
#include "string.h"	 
#include "timer.h"
//////////////////////////////////////////////////////////////////////////////////	 
//±¾³ÌÐòÖ»¹©Ñ§Ï°Ê¹ÓÃ£¬Î´¾­×÷ÕßÐí¿É£¬²»µÃÓÃÓÚÆäËüÈÎºÎÓÃÍ¾
//ALIENTEK STM32¿ª·¢°å
//´®¿Ú3Çý¶¯´úÂë	   
//ÕýµãÔ­×Ó@ALIENTEK
//¼¼ÊõÂÛÌ³:www.openedv.com
//ÐÞ¸ÄÈÕÆÚ:2014/3/29
//°æ±¾£ºV1.0
//°æÈ¨ËùÓÐ£¬µÁ°æ±Ø¾¿¡£
//Copyright(C) ¹ãÖÝÊÐÐÇÒíµç×Ó¿Æ¼¼ÓÐÏÞ¹«Ë¾ 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	   

//´®¿Ú½ÓÊÕ»º´æÇø 	
u8 USART3_RX_BUF[USART3_MAX_RECV_LEN]; 				//½ÓÊÕ»º³å,×î´óUSART3_MAX_RECV_LEN¸ö×Ö½Ú.
u8  USART3_TX_BUF[USART3_MAX_SEND_LEN]; 			//·¢ËÍ»º³å,×î´óUSART3_MAX_SEND_LEN×Ö½Ú

//Í¨¹ýÅÐ¶Ï½ÓÊÕÁ¬Ðø2¸ö×Ö·ûÖ®¼äµÄÊ±¼ä²î²»´óÓÚ10msÀ´¾ö¶¨ÊÇ²»ÊÇÒ»´ÎÁ¬ÐøµÄÊý¾Ý.
//Èç¹û2¸ö×Ö·û½ÓÊÕ¼ä¸ô³¬¹ý10ms,ÔòÈÏÎª²»ÊÇ1´ÎÁ¬ÐøÊý¾Ý.Ò²¾ÍÊÇ³¬¹ý10msÃ»ÓÐ½ÓÊÕµ½
//ÈÎºÎÊý¾Ý,Ôò±íÊ¾´Ë´Î½ÓÊÕÍê±Ï.
//½ÓÊÕµ½µÄÊý¾Ý×´Ì¬
//[15]:0,Ã»ÓÐ½ÓÊÕµ½Êý¾Ý;1,½ÓÊÕµ½ÁËÒ»ÅúÊý¾Ý.
//[14:0]:½ÓÊÕµ½µÄÊý¾Ý³¤¶:È
vu16 USART3_RX_STA=0;   	


void USART3_IRQHandler(void)
{
	u8 res;	      
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)//½ÓÊÕµ½Êý¾Ý
	{	 
		res =USART_ReceiveData(USART3);		 
		if((USART3_RX_STA&(1<<15))==0)//½ÓÊÕÍêµÄÒ»ÅúÊý¾Ý,»¹Ã»ÓÐ±»´¦Àí,Ôò²»ÔÙ½ÓÊÕÆäËûÊý¾Ý
		{ 
			if(USART3_RX_STA<USART3_MAX_RECV_LEN)	//»¹¿ÉÒÔ½ÓÊÕÊý¾Ý
			{
				TIM_SetCounter(TIM7,0);//¼ÆÊýÆ÷Çå¿Õ          				//¼ÆÊýÆ÷Çå¿Õ
				if(USART3_RX_STA==0) 				//Ê¹ÄÜ¶¨Ê±Æ÷7µÄÖÐ¶Ï 
				{
					TIM_Cmd(TIM7,ENABLE);//Ê¹ÄÜ¶¨Ê±Æ÷7
				}
				USART3_RX_BUF[USART3_RX_STA++]=res;	//¼ÇÂ¼½ÓÊÕµ½µÄÖµ	 
			}else 
			{
				USART3_RX_STA|=1<<15;				//Ç¿ÖÆ±ê¼Ç½ÓÊÕÍê³É
			} 
		}
	}  				 											 
}   


//³õÊ¼»¯IO ´®¿Ú3
//pclk1:PCLK1Ê±ÖÓÆµÂÊ(Mhz)
//bound:²¨ÌØÂÊ	  
void usart3_init(u32 bound)
{  

	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	// GPIOBÊ±ÖÓ
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE); //´®¿Ú3Ê±ÖÓÊ¹ÄÜ

 	USART_DeInit(USART3);  //¸´Î»´®¿Ú3
		 //USART3_TX   PB10
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PB10
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//¸´ÓÃÍÆÍìÊä³ö
  GPIO_Init(GPIOB, &GPIO_InitStructure); //³õÊ¼»¯PB10
   
    //USART3_RX	  PB11
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//¸¡¿ÕÊäÈë
  GPIO_Init(GPIOB, &GPIO_InitStructure);  //³õÊ¼»¯PB11
	
	USART_InitStructure.USART_BaudRate = bound;//²¨ÌØÂÊÒ»°ãÉèÖÃÎª9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//×Ö³¤Îª8Î»Êý¾Ý¸ñÊ½
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//Ò»¸öÍ£Ö¹Î»
	USART_InitStructure.USART_Parity = USART_Parity_No;//ÎÞÆæÅ¼Ð£ÑéÎ»
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//ÎÞÓ²¼þÊý¾ÝÁ÷¿ØÖÆ
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//ÊÕ·¢Ä£Ê½
  
	USART_Init(USART3, &USART_InitStructure); //³õÊ¼»¯´®¿Ú	3
  

	USART_Cmd(USART3, ENABLE);                    //Ê¹ÄÜ´®¿Ú 
	
	//Ê¹ÄÜ½ÓÊÕÖÐ¶Ï
  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//¿ªÆôÖÐ¶Ï   
	
	//ÉèÖÃÖÐ¶ÏÓÅÏÈ¼¶
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;//ÇÀÕ¼ÓÅÏÈ¼¶3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//×ÓÓÅÏÈ¼¶3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQÍ¨µÀÊ¹ÄÜ
	NVIC_Init(&NVIC_InitStructure);	//¸ù¾ÝÖ¸¶¨µÄ²ÎÊý³õÊ¼»¯VIC¼Ä´æÆ÷
	
	
	TIM7_Int_Init(1000-1,7200-1);		//10msÖÐ¶Ï
	USART3_RX_STA=0;		//ÇåÁã
	TIM_Cmd(TIM7,DISABLE);			//¹Ø±Õ¶¨Ê±Æ÷7

}

//´®¿Ú3,printf º¯Êý
//È·±£Ò»´Î·¢ËÍÊý¾Ý²»³¬¹ýUSART3_MAX_SEND_LEN×Ö½Ú
void u3_printf(char* fmt,...)  
{  
	u16 i,j; 
	va_list ap; 
	va_start(ap,fmt);
	vsprintf((char*)USART3_TX_BUF,fmt,ap);
	va_end(ap);
	i=strlen((const char*)USART3_TX_BUF);		//´Ë´Î·¢ËÍÊý¾ÝµÄ³¤¶È
	for(j=0;j<i;j++)							//Ñ­»··¢ËÍÊý¾Ý
	{
	  while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET); //Ñ­»··¢ËÍ,Ö±µ½·¢ËÍÍê±Ï   
		USART_SendData(USART3,USART3_TX_BUF[j]); 
	} 
}

 



















