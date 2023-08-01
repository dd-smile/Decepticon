#ifndef __ESP8266_APPLY_H__
#define __ESP8266_APPLY_H__

#include "stm32f4xx.h"


/********************************** 用户需要设置的参数**********************************/
#define      macUser_ESP8266_ApSsid                       "avant-ap"                //要连接的热点的名称
#define      macUser_ESP8266_ApPwd                        "12345678"           //要连接的热点的密钥

#define      macUser_ESP8266_TcpServer_IP                 "broker.emqx.io "      //要连接的服务器的 IP     //192.168.20.243     112.74.105.251   //broker.emqx.io
#define      macUser_ESP8266_TcpServer_Port               "1883"               //要连接的服务器的端口

#define      ESP8266_WIFI_INFO		                      "AT+CWJAP=\"iPhone 11\",\"7815csdd\"\r\n"
#define      ESP8266_AVANTNET_INFO                        "AT+CIPSTART=\"TCP\",\"112.74.105.251\",1883\r\n"


extern volatile uint8_t ucTcpClosedFlag;


void ESP8266_MQTTConnection(void);
void ESP8266_CheckRecvDataTest(void);

#endif
