#include "stm32f4xx.h"
#include "usart.h"
#include "delay.h"
#include "exti.h"
#include "lcd.h"
#include "led.h"
#include "exti.h"
#include "key.h"
#include "adc.h"
#include "usart3.h"
#include "common.h"
#include "rtc.h"

uint8_t LED_Flag = 0;
uint8_t sendDataFlag = 0;
uint8_t timeDisplay = 0;
uint8_t str[200];


int main(void)
{
	RTC_TimeTypeDef RTC_TimeStruct;
	RTC_DateTypeDef RTC_DateStruct;
	uint16_t adcx = 0;
	float temp = 0;
	u8 tbuf[40]; 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	uart_init(115200);
	usart3_init(115200);
	delay_init(84);
	
	LED_Init(); 
	KEY_Init();
	Adc_Init();
	EXTIX_Init();
	LCD_Init();
	POINT_COLOR = RED;
	
//	u3_printf("Hello\r\n");
	
	while (atk_8266_send_cmd("AT", "OK", 20));			/* 检测WiFi是否已经连接 */
/******************************************************************************/
LCD_ShowString(30,40,210,24,24,"Explorer STM32F4");	
/******************************************************************************/	
	atk_8266_at_response(1);
	while (atk_8266_send_cmd("AT+CWMODE=1", "OK", 20));		/* 将WiFi设置为STA模式 */
	atk_8266_at_response(1);
	while (atk_8266_send_cmd("AT+CWJAP=\"514515\",\"514515123\"", "OK", 20));				/* 连接WiFi */
	atk_8266_at_response(1);
	while (atk_8266_send_cmd("AT+CIPSTART=\"TCP\",\"192.168.199.101\",8086", "OK", 20));	/* 连接到服务器 */
//	while (atk_8266_send_cmd("AT+CIPSTART=\"TCP\",\"119.29.80.76\",8090", "OK", 20));
	atk_8266_at_response(1);
	while (atk_8266_send_cmd("AT+CIPMODE=1", "OK", 20));				/* 开启透传模式 */
	atk_8266_at_response(1);
	while (atk_8266_send_cmd("AT+CIPSEND", "OK", 20));				/* 开始透传 */
	atk_8266_at_response(1);
	
	My_RTC_Init();
	RTC_Set_WakeUp(RTC_WakeUpClock_CK_SPRE_16bits,0);
	
	while(1){
		LED0 = 1;
		
		
		if (sendDataFlag == 1) {
			sendDataFlag = 0;
			u3_printf("%s", str);
		}
		if (timeDisplay == 1) {
			adcx = Get_Adc_Average(ADC_Channel_1, 10);
			sprintf((char *)str, "ADC_VAL:%d", adcx);
			LCD_ShowString(60, 130, 200, 16, 16, str);	/* 显示ADC的值 */

			temp = (float)adcx * (3.3 / 4096);
			adcx = temp;
			sprintf((char *)str, "ADC_VOL:%.3fV", temp);
			LCD_ShowString(60, 150, 200, 16, 16, str);	/* 显示电压值 */
			timeDisplay = 0;
			RTC_GetTime(RTC_Format_BIN,&RTC_TimeStruct);
			sprintf((char*)tbuf,"Time:%02d:%02d:%02d",RTC_TimeStruct.RTC_Hours,RTC_TimeStruct.RTC_Minutes,RTC_TimeStruct.RTC_Seconds); 
			LCD_ShowString(30,210,210,16,16,tbuf);	
		}
		
		atk_8266_at_response(1);
		if (LED_Flag == 1) {
			LED1 = !LED1;
		}
	}
}
