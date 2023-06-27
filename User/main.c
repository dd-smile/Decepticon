#include "stm32f4xx.h"
#include "./usart/bsp_debug_usart.h"
#include "./systick/bsp_SysTick.h"
#include "DHT11/bsp_dht11.h"
#include "iic.h"
#include "sht35.h"
#include "bsp_led.h"


int main(void)
{	

	Debug_USART_Config();//串口初始化

	SysTick_Init();//定时初始化

	LED_GPIO_Config();//RGB灯初始化
	
	
	while(1)
	{
		//printf("\r\nSHT35\r\n");
		//Delay_ms(1000);
		//iic_Test();
		//SHT35_init();
		LED1( ON );      // 亮
		Delay_ms(1000);
        LED1( OFF );      // 灭
		
        LED2( ON );     // 亮
		Delay_ms(1000);
        LED2( OFF );      // 灭

        LED3( ON );      // 亮
		Delay_ms(1000);
        LED3( OFF );      // 灭

        /*轮流显示 红绿蓝黄紫青白 颜色*/
        LED_RED;
		Delay_ms(1000);

        LED_GREEN;
		Delay_ms(1000);

        LED_BLUE;
		Delay_ms(1000);

        LED_YELLOW;
		Delay_ms(1000);

        LED_PURPLE;
		Delay_ms(1000);

        LED_CYAN;
		Delay_ms(1000);

        LED_WHITE;
		Delay_ms(1000);

        LED_RGBOFF;
		Delay_ms(1000);



	} 




}




/*********************************************END OF FILE**********************/

