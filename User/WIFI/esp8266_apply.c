#include "esp8266_apply.h"
#include "esp8266.h"
#include "./dwt_delay/core_delay.h"
#include <stdio.h>  
#include <string.h>  
#include <stdbool.h>

#include "./led/bsp_led.h"
#include "./usart/bsp_debug_usart.h"

#define LED_CMD_NUMBER   8
char *ledCmd[ LED_CMD_NUMBER ] = { "LED_RED","LED_GREEN","LED_BLUE","LED_YELLOW","LED_PURPLE","LED_CYAN","LED_WHITE","LED_RGBOFF" };

volatile uint8_t ucTcpClosedFlag = 0;


/*
 * 函数名：Get_ESP82666_Cmd
 * 描述   获取网络调试助手和串口发来的信息
 * 输入  ：无
 * 返回  : 无
 */
void Get_ESP82666_Cmd( char * cmd)
{
	uint8_t i;
	for(i = 0;i < LED_CMD_NUMBER; i++)
	{
     if(( bool ) strstr ( cmd, ledCmd[i] ))
		 break;
	}
	switch(i)
    {
      case 0:
        LED_RED;
      break;
      case 1:
        LED_GREEN;
      break;
      case 2:
        LED_BLUE;
      break;
      case 3:
        LED_YELLOW;
      break;
      case 4:
        LED_PURPLE;
      break;
      case 5:
        LED_CYAN;
      break;
      case 6:
        LED_WHITE;
      break;
      case 7:
        LED_RGBOFF;
      break;
      default:
        
        break;      
    }   
}



/*
 * 函数名：ESP8266_MQTTConnection
 * 描述   配置ESP8266,进行MQTT连接
 * 输入  ：无
 * 返回  : 无
 * 调用  ：被外部调用
 */
void ESP8266_MQTTConnection(void)
{
    
	macESP8266_CH_ENABLE();
	while( ! ESP8266_AT_Test() );
	Delay_ms(500);
	while( ! ESP8266_Net_Mode_Choose ( STA ) );
	Delay_ms(500);
  ESP8266_DHCP_CUR();
  Delay_ms(500);
  while( ! ESP8266_JoinAP ( macUser_ESP8266_ApSsid, macUser_ESP8266_ApPwd ) );	   //加入WiFi
	Delay_ms(500);
  Delay_ms(1000);
  Delay_ms(1000);
  Delay_ms(1000);
  Delay_ms(1000);
  Delay_ms(1000);
  Delay_ms(1000);
  Delay_ms(1000);
  Delay_ms(1000);
  Delay_ms(1000);
  Delay_ms(1000);
	while( ! ESP8266_Link_Server ( enumTCP, macUser_ESP8266_TcpServer_IP, macUser_ESP8266_TcpServer_Port ) );  //连接服务器
	Delay_ms(500);
	while( ! ESP8266_UnvarnishSend () );    //进入透传发送模式

	
	printf( "\r\nset ESP8266 ok\r\n" );

}



/**
  * @brief  ESP8266 检查是否接收到了数据，检查连接和掉线重连
  * @param  无
  * @retval 无
  * 数据流向:  串口发送AT指令:电脑串口助手->USART1接收->USART3发送->ESP8266
  * 		  ESP8266返回消息:ESP8266->USART3接收->USART1发送->电脑串口助手
  */
void ESP8266_CheckRecvDataTest(void)
{
  uint8_t ucStatus;
  uint16_t i;
  
  /* 如果接收到了串口调试助手的数据 */
  if(strUSART_Fram_Record.InfBit.FramFinishFlag == 1)
  {
    for(i = 0;i < strUSART_Fram_Record.InfBit.FramLength; i++)
    {
       USART_SendData( macESP8266_USARTx ,strUSART_Fram_Record.Data_RX_BUF[i]); //转发给ESP8266
       while(USART_GetFlagStatus(macESP8266_USARTx,USART_FLAG_TC)==RESET){}      //等待发送完成
    }
    strUSART_Fram_Record .InfBit .FramLength = 0;                                //接收数据长度置零
    strUSART_Fram_Record .InfBit .FramFinishFlag = 0;                            //接收标志置零
    Get_ESP82666_Cmd(strUSART_Fram_Record .Data_RX_BUF);                         //检查一下是不是点灯命令
  }
  
  /* 如果接收到了ESP8266的数据 */
  if(strEsp8266_Fram_Record.InfBit.FramFinishFlag)
  {                                                      
    for(i = 0;i < strEsp8266_Fram_Record .InfBit .FramLength; i++)               
    {
       USART_SendData( DEBUG_USART ,strEsp8266_Fram_Record .Data_RX_BUF[i]);    //转发给串口3
       while(USART_GetFlagStatus(DEBUG_USART,USART_FLAG_TC)==RESET){}
    }
     strEsp8266_Fram_Record .InfBit .FramLength = 0;                             //接收数据长度置零
     strEsp8266_Fram_Record.InfBit.FramFinishFlag = 0;                           //接收标志置零
     Get_ESP82666_Cmd(strEsp8266_Fram_Record .Data_RX_BUF);                      //检查一下是不是点灯命令
  }
  
  if ( ucTcpClosedFlag )                                             //检测是否失去连接
  {
    ESP8266_ExitUnvarnishSend ();                                    //退出透传模式
    
    do ucStatus = ESP8266_Get_LinkStatus ();                         //获取连接状态
    while ( ! ucStatus );
    
    if ( ucStatus == 4 )                                             //确认失去连接后重连
    {
      printf ( "\r\nReconnecting the hotspot and server ......\r\n" );
      
      while ( ! ESP8266_JoinAP ( macUser_ESP8266_ApSsid, macUser_ESP8266_ApPwd ) );
      
      while ( !	ESP8266_Link_Server ( enumTCP, macUser_ESP8266_TcpServer_IP, macUser_ESP8266_TcpServer_Port ) );
      
      printf ( "\r\nReconnecting the hotspot and server succeeded.\r\n" );

    }
    
    while ( ! ESP8266_UnvarnishSend () );		
    
  }
}

