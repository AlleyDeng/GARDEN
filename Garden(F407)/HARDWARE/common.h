#ifndef __COMMON_H__
#define __COMMON_H__
#include "sys.h"
#include "usart.h"
#include "delay.h"
#include "led.h"
#include "key.h"
#include "lcd.h"
#include "usart3.h"
#include "string.h"

void atk_8266_at_response(u8 mode);
u8* atk_8266_check_cmd(u8 *str);
u8 atk_8266_send_data(u8 *data,u8 *ack,u16 waittime);
u8 atk_8266_send_cmd(u8 *cmd,u8 *ack,u16 waittime);
u8 atk_8266_quit_trans(void);

extern u16 USART3_RX_STA;
extern u8 USART3_RX_BUF[];
//extern u8* kbd_fn_tbl[2];
//extern const u8* ATK_ESP8266_CWMODE_TBL[3];
//extern const u8* ATK_ESP8266_WORKMODE_TBL[3];
//extern const u8* ATK_ESP8266_ECN_TBL[5];
#endif





