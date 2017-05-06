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
#include "rtc.h"
#include "ds18b20.h"

uint8_t LED_Flag = 0;
uint8_t waterFlag = 0;
uint8_t sendDataFlag = 0;
uint8_t timeDisplay = 0;
uint8_t ctrlFlag = 0;
uint8_t str[200];
short temperature = 0;
float adcStandard = 2.5;

int main(void)
{	 
	float temp = 0;
//	uint8_t key = 0;
	uint16_t adcx = 0;
	
	delay_init();	    	 	//��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 		//���ڳ�ʼ��Ϊ115200
	LED_Init();		  			//��ʼ����LED���ӵ�Ӳ���ӿ�
	KEY_Init();					//��ʼ������
	Adc_Init();
	RTC_Init();
	EXTIX_Init();
	LCD_Init();			   	   	//��ʼ��LCD  
	usart3_init(115200);		//��ʼ������3 
	
	POINT_COLOR=RED;
	
	GPIO_ResetBits(GPIOE, GPIO_Pin_1);
	
	/* ds18b20��ʼ�� */
	while(DS18B20_Init());	
	
	while (atk_8266_send_cmd("AT", "OK", 20));				/* ���WiFi�Ƿ��Ѿ����� */
	atk_8266_at_response(1);
	while (atk_8266_send_cmd("AT+CWMODE=1", "OK", 20));		/* ��WiFi����ΪSTAģʽ */
	atk_8266_at_response(1);
//	while (atk_8266_send_cmd("AT+CWJAP=\"514515\",\"514515123\"", "OK", 1000));				/* ����WiFi */
	while (atk_8266_send_cmd("AT+CWJAP=\"chao\",\"12345678\"", "OK",1000));
	atk_8266_at_response(1);
//	while (atk_8266_send_cmd("AT+CIPSTART=\"TCP\",\"172.16.118.128\",8086", "OK", 1000));	/* ���ӵ������� */
	while (atk_8266_send_cmd("AT+CIPSTART=\"TCP\",\"119.29.80.76\",8090", "OK", 20));
	atk_8266_at_response(1);
	while (atk_8266_send_cmd("AT+CIPMODE=1", "OK", 20));				/* ����͸��ģʽ */
	atk_8266_at_response(1);
	while (atk_8266_send_cmd("AT+CIPSEND", "OK", 20));				/* ��ʼ͸�� */
	atk_8266_at_response(1);

	while (1) {
		GPIO_ResetBits(GPIOB, GPIO_Pin_5);
		
		temperature = DS18B20_Get_Temp();	
		sprintf((char *)str, "TEMPTURE:%.1f", temperature/10.0);
		LCD_ShowString(40, 330, 200, 16, 16, str);
		
		adcx = Get_Adc_Average(ADC_Channel_1, 10);
        sprintf((char *)str, "ADC_VAL:%d", adcx);
        LCD_ShowString(60, 350, 200, 16, 16, str);	/* ��ʾADC��ֵ */

        temp = (float)adcx * (3.3 / 4096);
        adcx = temp;
		if (adcx > adcStandard && ctrlFlag == 1) {
				waterFlag = 1;
				LED_Flag = 1;
		}
        sprintf((char *)str, "ADC_VOL:%.3fV", temp);
        LCD_ShowString(60, 370, 200, 16, 16, str);	/* ��ʾ��ѹֵ */
		
		if (ctrlFlag == 1) {
			if (adcx > adcStandard) {
				waterFlag = 1;
			}
			ctrlFlag = 0;
		}
		
		if (sendDataFlag == 1) {
			sendDataFlag = 0;
			u3_printf("%s", str);
		}
		if (timeDisplay == 1) {
			timeDisplay = 0;
			TimeDisplay(RTC_GetCounter());
		}
		if (LED_Flag == 1) {
			GPIO_ResetBits(GPIOE, GPIO_Pin_5);
		} else if (LED_Flag == 0) {
			GPIO_SetBits(GPIOE, GPIO_Pin_5);
		}
		if (waterFlag == 1) {
			GPIO_ResetBits(GPIOE, GPIO_Pin_1);		
		}	else {
			GPIO_SetBits(GPIOE, GPIO_Pin_1);		
		}
		atk_8266_at_response(1);
	}
}


















