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
	
	delay_init();	    	 	//��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 		//���ڳ�ʼ��Ϊ115200
	LED_Init();		  			//��ʼ����LED���ӵ�Ӳ���ӿ�
	KEY_Init();					//��ʼ������
	LCD_Init();			   	   	//��ʼ��LCD  
	usart3_init(115200);		//��ʼ������3 
	
	POINT_COLOR=RED;
	
	while (atk_8266_send_cmd("AT", "OK", 20));				/* ���WiFi�Ƿ��Ѿ����� */
	atk_8266_at_response(1);
	while (atk_8266_send_cmd("AT+CWMODE=1", "OK", 20));		/* ��WiFi����ΪSTAģʽ */
	atk_8266_at_response(1);
	while (atk_8266_send_cmd("AT+CWJAP=\"514515\",\"514515123\"", "OK", 20));				/* ����WiFi */
	atk_8266_at_response(1);
	while (atk_8266_send_cmd("AT+CIPSTART=\"TCP\",\"192.168.199.101\",8086", "OK", 20));	/* ���ӵ������� */
//	while (atk_8266_send_cmd("AT+CIPSTART=\"TCP\",\"119.29.80.76\",8086", "OK", 20));
	atk_8266_at_response(1);
	
	while(DS18B20_Init())	//DS18B20��ʼ��	
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


















