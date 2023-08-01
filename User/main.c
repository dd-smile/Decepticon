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
#include "esp8266_apply.h"

//传感器
#include "DHT11/bsp_dht11.h"
#include "sht35.h"

//通信协议
#include "iic.h"
#include "avantnet.h"

//C库
#include <stdio.h>


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

  ESP8266_Init();//WIFI模块初始化(使用的接口和外设)

	char PUB_BUF[256];    //上传数据的BUF
	uint8_t temp = 26,hum = 30;
	uint32_t CO2Data = 500;


	ESP8266_MQTTConnection();

	while(AvantNet_DevLink())
	Delay_ms(500);


	while(1)
	{
		
		//ESP8266_CheckRecvDataTest();

		//printf("\r\nSHT35\r\n");
		//Delay_ms(1000);
		//SHT35_init();   //SHT35串口显示温湿度数据
		//LED_RGB_Init();   //LED_RGB循环显示流水灯
		//Relay_Init();//控制继电器启动

		//  UsartPrintf(DEBUG_USART,"EMQX_Public");
		//  sprintf(PUB_BUF,"{\"Temp\":%d,\"Hum\":%d,\"Co2\":%d}",temp,hum,CO2Data);
		//  AvantNet_Publish("/avant/pub", PUB_BUF);
		//  Delay_ms(1000);
		//  Delay_ms(1000);
		//  Delay_ms(1000);
		//  Delay_ms(1000);
		//  Delay_ms(1000);
		//  Delay_ms(1000);
		//  Delay_ms(1000);

	} 

	

}




/*********************************************END OF FILE**********************/

