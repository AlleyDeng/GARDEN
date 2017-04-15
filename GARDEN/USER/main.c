#include "sys.h"
#include "delay.h"
#include "usart.h" 
#include "led.h" 		 	  
#include "key.h"
#include "adc.h"
#include "usart3.h"
#include "common.h" 
#include "string.h"
#include "stdint.h"
#include "exti.h"

uint8_t LED_Flag = 0;
uint8_t sendDataFlag = 0;
uint8_t str[200];

int main(void)
{	 
//	float temp = 0;
//	uint8_t key = 0;
//	uint16_t adcx = 0;
	
	delay_init();	    	 	//延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	uart_init(115200);	 		//串口初始化为115200
	LED_Init();		  			//初始化与LED连接的硬件接口
	KEY_Init();					//初始化按键
	EXTIX_Init();
	LCD_Init();			   	   	//初始化LCD  
	usart3_init(115200);		//初始化串口3 
	
	POINT_COLOR=RED;
	
	while (atk_8266_send_cmd("AT", "OK", 20));				/* 检测WiFi是否已经连接 */
	atk_8266_at_response(1);
	while (atk_8266_send_cmd("AT+CWMODE=1", "OK", 20));		/* 将WiFi设置为STA模式 */
	atk_8266_at_response(1);
	while (atk_8266_send_cmd("AT+CWJAP=\"514515\",\"514515123\"", "OK", 20));				/* 连接WiFi */
	atk_8266_at_response(1);
//	while (atk_8266_send_cmd("AT+CIPSTART=\"TCP\",\"192.168.199.101\",8086", "OK", 20));	/* 连接到服务器 */
	while (atk_8266_send_cmd("AT+CIPSTART=\"TCP\",\"119.29.80.76\",8086", "OK", 20));
	atk_8266_at_response(1);
	while (atk_8266_send_cmd("AT+CIPMODE=1", "OK", 20));				/* 开启透传模式 */
	atk_8266_at_response(1);
	while (atk_8266_send_cmd("AT+CIPSEND", "OK", 20));				/* 开始透传 */
	atk_8266_at_response(1);
	
	while (1) {
		GPIO_ResetBits(GPIOB, GPIO_Pin_5);

		if (sendDataFlag == 1) {
			sendDataFlag = 0;
			u3_printf("Hello\n");
		}
		
//		adcx = Get_Adc_Average(ADC_Channel_1, 10);
//        sprintf((char *)str, "ADC_CH0_VAL:%d", adcx);
//        LCD_ShowString(60, 130, 200, 16, 16, str);	/* 显示ADC的值 */

//        temp = (float)adcx * (3.3 / 4096);
//        adcx = temp;
//        sprintf((char *)str, "ADC_CH0_VOL:%.3fV", temp);
//        LCD_ShowString(60, 150, 200, 16, 16, str);	/* 显示电压值 */

//		LCD_ShowString(20, 150, 200, 16, 16, "ADC:");

//LCD_ShowString(30,40,210,24,24,"Elite STM32F1 ^_^");
		atk_8266_at_response(1);
		if (LED_Flag == 1) {
			GPIO_ResetBits(GPIOE, GPIO_Pin_5);
		} else if (LED_Flag == 0) {
			GPIO_SetBits(GPIOE, GPIO_Pin_5);
		}
	}
}


















