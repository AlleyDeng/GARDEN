#include "wifi.h"

//���Ӷ˿ں�:8086,�������޸�Ϊ�����˿�.
const uint8_t* portnum = "8086";		 

//WIFI STAģʽ,����Ҫȥ���ӵ�·�������߲���,��������Լ���·��������,�����޸�.
const uint8_t* wifista_ssid = "514515";			//·����SSID��
const uint8_t* wifista_encryption = "wpawpa2_aes";	//wpa/wpa2 aes���ܷ�ʽ
const uint8_t* wifista_password = "514515123"; 	//��������

//WIFI APģʽ,ģ���������߲���,�������޸�.
const uint8_t* wifiap_ssid = "ATK-ESP8266";			//����SSID��
const uint8_t* wifiap_encryption = "wpawpa2_aes";	//wpa/wpa2 aes���ܷ�ʽ
const uint8_t* wifiap_password = "12345678"; 		//�������� 

//4������ģʽ
const uint8_t* ATK_ESP8266_CWMODE_TBL[3]={"STAģʽ ","APģʽ ","AP&STAģʽ "};	//ATK-ESP8266,3������ģʽ,Ĭ��Ϊ·����(ROUTER)ģʽ 
//4�ֹ���ģʽ
const uint8_t *ATK_ESP8266_WORKMODE_TBL[3]={"TCP������","TCP�ͻ���"," UDP ģʽ"};	//ATK-ESP8266,4�ֹ���ģʽ
//5�ּ��ܷ�ʽ
const uint8_t *ATK_ESP8266_ECN_TBL[5]={"OPEN","WEP","WPA_PSK","WPA2_PSK","WPA_WAP2_PSK"};

/**
  *@brief 	�����յ������ݷ��ظ�����
  *@param 	mode	0,�����㣻1������
  *@retval	None
  */
void WIFI_Response(uint8_t mode)
{
	if (USART3_RXOVER == 1) {
		USART3_RXBUF[USART3_RXCUNT] ='\0';	/* ��ӽ����� */
		USART1_SendString(USART3_RXBUF);
	}
	if (mode == 1) {
		USART3_RXCUNT = 0;
		USART3_RXOVER = 0;
	}
}

/**
  *@brief	WiFi��������󣬼����յ���Ӧ��
  *@param 	str	�ڴ���Ӧ����
  *@retval	strx	0,û�еõ��ڴ���Ӧ����
  *					�������ڴ���Ӧ������λ��(str��λ��)
  */
uint8_t *WIFI_CheckCmd(uint8_t *str)
{
	char *strx = 0;
	if (USART3_RXOVER == 1) {
		USART3_RXBUF[USART3_RXCUNT] = '\0';
		strx = strstr((const char *)USART3_RXBUF, (const char *)str);
	}
	return (uint8_t *)strx;
}


/**
  *@brief 	��WiFi��������
  *@param 	cmd	���͵������ַ���
  *			ack	�ڴ���Ӧ����
  *			waittime	�ȴ���Ӧ��ʱ�䣨��λ 10ms��
  *@retval	res	0,���ͳɹ�(�õ��ڴ���Ӧ����)��1������ʧ��
  */
uint8_t WIFI_SendCmd(uint8_t *cmd, uint8_t *ack, uint8_t waittime)
{
	uint8_t res = 0;
	USART3_RXOVER = 0;
	u3_printf("%s\r\n", cmd);	/* �������� */
	if (ack && waittime) {		/* ��Ҫ�ȴ�Ӧ�� */
		while (--waittime) {	/* �ȴ�����ʱ */
			DelayMs(10);
			if (USART3_RXOVER == 1) {	/* ���յ��ڴ���Ӧ���� */
				if (WIFI_CheckCmd(ack)) {
					printf ("ack:%s\r\n", (uint8_t *)ack);
					break;
				}
				USART3_RXOVER = 0;
			}
		}
		if (waittime == 0) {
			res = 1;
		}
	}
	return res;
}

