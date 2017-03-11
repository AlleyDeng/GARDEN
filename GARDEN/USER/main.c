#include "sys.h"
#include "delay.h"
#include "usart.h" 
#include "led.h" 		 	  
#include "key.h"     	
#include "usart3.h"
#include "common.h" 
#include "string.h"
#include "stdint.h"
#include "ds18b20.h"

uint8_t LED_Flag = 0;
uint8_t sendDataFlag = 0;
float temperature = 0;
uint8_t str[200];

int main(void)
{	 
	uint8_t key;
	
	delay_init();	    	 	//延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	uart_init(115200);	 		//串口初始化为115200
	LED_Init();		  			//初始化与LED连接的硬件接口
	KEY_Init();					//初始化按键
	LCD_Init();			   	   	//初始化LCD  
	usart3_init(115200);		//初始化串口3 
	
	POINT_COLOR=RED;
	
	while (atk_8266_send_cmd("AT", "OK", 20));				/* 检测WiFi是否已经连接 */
	atk_8266_at_response(1);
	while (atk_8266_send_cmd("AT+CWMODE=1", "OK", 20));		/* 将WiFi设置为STA模式 */
	atk_8266_at_response(1);
	while (atk_8266_send_cmd("AT+CWJAP=\"514515\",\"514515123\"", "OK", 20));				/* 连接WiFi */
	atk_8266_at_response(1);
	while (atk_8266_send_cmd("AT+CIPSTART=\"TCP\",\"192.168.199.101\",8086", "OK", 20));	/* 连接到服务器 */
//	while (atk_8266_send_cmd("AT+CIPSTART=\"TCP\",\"119.29.80.76\",8086", "OK", 20));
	atk_8266_at_response(1);
	
	while(DS18B20_Init())	//DS18B20初始化	
	{
		LCD_ShowString(30,130,200,16,16,"DS18B20 Error");
		delay_ms(200);
		LCD_Fill(30,130,239,130+16,WHITE);
 		delay_ms(200);
	}	
	
	while (1) {
		GPIO_ResetBits(GPIOB, GPIO_Pin_5);
		
		temperature = DS18B20_Get_Temp() / 10;
		sprintf((char *)str, "temperature:%3.1fC", temperature);
		LCD_ShowString(30, 130, 200, 16, 16, str);
		
		key = KEY_Scan(0);
		if (key == 4) {
			sendDataFlag = 1;
		}
		if (sendDataFlag == 1) {
			sendDataFlag = 0;
			while (atk_8266_send_cmd("AT+CIPSEND=17", "OK", 20));	
			atk_8266_at_response(1);
			while (atk_8266_send_data(str, "OK", 20));
			atk_8266_at_response(1);
		}
		if (key == 1) {
			while (atk_8266_send_cmd("AT+CIPSEND=5", "OK", 20));	
			atk_8266_at_response(1);
			while (atk_8266_send_data("World", "OK", 20));
			atk_8266_at_response(1);
		}
		atk_8266_at_response(1);
		if (LED_Flag == 1) {
			GPIO_ResetBits(GPIOE, GPIO_Pin_5);
		} else if (LED_Flag == 0) {
			GPIO_SetBits(GPIOE, GPIO_Pin_5);
		}
	}
}


















