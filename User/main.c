#include "stm32f4xx.h"
#include "./usart/bsp_debug_usart.h"
#include "./systick/bsp_SysTick.h"
#include "DHT11/bsp_dht11.h"
#include "iic.h"
#include "sht35.h"
#include "bsp_led.h"
#include "bsp_key.h"
#include "bsp_relay.h"


int main(void)
{	

	Debug_USART_Config();//串口初始化

	SysTick_Init();//系统定时器初始化

	LED_GPIO_Config();//RGB灯初始化

	Key_GPIO_Config();//按键初始化

	RELAY_GPIO_Config();//继电器IO初始化
	
	
	
	while(1)
	{
		//printf("\r\nSHT35\r\n");
		//Delay_ms(1000);
		//SHT35_init();   //SHT35串口显示温湿度数据
		//LED_RGB_Init();   //LED_RGB循环显示流水灯
		Relay_Init();//控制继电器启动



	} 




}




/*********************************************END OF FILE**********************/

