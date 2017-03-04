#include "wifi.h"

//连接端口号:8086,可自行修改为其他端口.
const uint8_t* portnum = "8086";		 

//WIFI STA模式,设置要去连接的路由器无线参数,请根据你自己的路由器设置,自行修改.
const uint8_t* wifista_ssid = "514515";			//路由器SSID号
const uint8_t* wifista_encryption = "wpawpa2_aes";	//wpa/wpa2 aes加密方式
const uint8_t* wifista_password = "514515123"; 	//连接密码

//WIFI AP模式,模块对外的无线参数,可自行修改.
const uint8_t* wifiap_ssid = "ATK-ESP8266";			//对外SSID号
const uint8_t* wifiap_encryption = "wpawpa2_aes";	//wpa/wpa2 aes加密方式
const uint8_t* wifiap_password = "12345678"; 		//连接密码 

//4个网络模式
const uint8_t* ATK_ESP8266_CWMODE_TBL[3]={"STA模式 ","AP模式 ","AP&STA模式 "};	//ATK-ESP8266,3种网络模式,默认为路由器(ROUTER)模式 
//4种工作模式
const uint8_t *ATK_ESP8266_WORKMODE_TBL[3]={"TCP服务器","TCP客户端"," UDP 模式"};	//ATK-ESP8266,4种工作模式
//5种加密方式
const uint8_t *ATK_ESP8266_ECN_TBL[5]={"OPEN","WEP","WPA_PSK","WPA2_PSK","WPA_WAP2_PSK"};

/**
  *@brief 	将接收到的数据返回给串口
  *@param 	mode	0,不清零；1，清零
  *@retval	None
  */
void WIFI_Response(uint8_t mode)
{
	if (USART3_RXOVER == 1) {
		USART3_RXBUF[USART3_RXCUNT] ='\0';	/* 添加结束符 */
		USART1_SendString(USART3_RXBUF);
	}
	if (mode == 1) {
		USART3_RXCUNT = 0;
		USART3_RXOVER = 0;
	}
}

/**
  *@brief	WiFi发送命令后，检测接收到的应答
  *@param 	str	期待的应答结果
  *@retval	strx	0,没有得到期待的应答结果
  *					其他，期待的应答结果的位置(str的位置)
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
  *@brief 	向WiFi发送命令
  *@param 	cmd	发送的命令字符串
  *			ack	期待的应答结果
  *			waittime	等待的应答时间（单位 10ms）
  *@retval	res	0,发送成功(得到期待的应答结果)；1，发送失败
  */
uint8_t WIFI_SendCmd(uint8_t *cmd, uint8_t *ack, uint8_t waittime)
{
	uint8_t res = 0;
	USART3_RXOVER = 0;
	u3_printf("%s\r\n", cmd);	/* 发送命令 */
	if (ack && waittime) {		/* 需要等待应答 */
		while (--waittime) {	/* 等待倒计时 */
			DelayMs(10);
			if (USART3_RXOVER == 1) {	/* 接收到期待的应答结果 */
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

