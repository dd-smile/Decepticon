//单片机头文件
#include "stm32f4xx.h"
#include "stm32f4xx_it.h"

//板载硬件驱动
#include "./usart/bsp_debug_usart.h"
#include "./dwt_delay/core_delay.h"
#include "bsp_led.h"
#include "bsp_key.h"
#include "bsp_relay.h"

//硬件驱动
#include "esp8266.h"

//传感器
#include "DHT11/bsp_dht11.h"
#include "sht35.h"

//通信协议
#include "iic.h"

/* MPU6050数据 */
short Acel[3];
short Gyro[3];
float Temp;

int main(void)
{	

	Debug_USART_Config();//串口初始化

	// SysTick_Init();//系统定时器初始化
	// SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;//启动定时器

	CPU_TS_TmrInit(); //初始化DWT计数器

	LED_GPIO_Config();//RGB灯初始化

	Key_GPIO_Config();//按键初始化

	RELAY_GPIO_Config();//继电器IO初始化

  	ESP8266_Init();//WIFI模块初始化

  	macESP8266_CH_ENABLE();//使能

	while(1)
	{
		printf("\r\nSHT35\r\n");
		Delay_ms(1000);
		SHT35_init();   //SHT35串口显示温湿度数据
		//LED_RGB_Init();   //LED_RGB循环显示流水灯
		//Relay_Init();//控制继电器启动
		
//    if(strUSART_Fram_Record.InfBit.FramFinishFlag == 1)  //如果接收到了串口调试助手的数据
//		{
//			strUSART_Fram_Record.Data_RX_BUF[strUSART_Fram_Record.InfBit.FramLength] = '\0';
//			Usart_SendString(macESP8266_USARTx ,strUSART_Fram_Record.Data_RX_BUF);      //数据从串口调试助手转发到ESP8266
//			strUSART_Fram_Record.InfBit .FramLength = 0;                                //接收数据长度置零
//			strUSART_Fram_Record.InfBit .FramFinishFlag = 0;                            //接收标志置零
//	    }
//		if(strEsp8266_Fram_Record.InfBit .FramFinishFlag)                             //如果接收到了ESP8266的数据
//		{                                                      
//			 strEsp8266_Fram_Record.Data_RX_BUF[strEsp8266_Fram_Record.InfBit.FramLength] = '\0';
//			 Usart_SendString(DEBUG_USART ,strEsp8266_Fram_Record.Data_RX_BUF);        //数据从ESP8266转发到串口调试助手
//			 strEsp8266_Fram_Record.InfBit.FramLength = 0;                             //接收数据长度置零
//			 strEsp8266_Fram_Record.InfBit.FramFinishFlag = 0;                           //接收标志置零
//		}

	} 

	

}




/*********************************************END OF FILE**********************/

