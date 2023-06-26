#include "stm32f4xx.h"
#include "./usart/bsp_debug_usart.h"
#include "./systick/bsp_SysTick.h"
#include "DHT11/bsp_dht11.h"
#include "iic.h"
#include "sht35.h"


int main(void)
{	

	Debug_USART_Config();

	SysTick_Init();
	
	
	while(1)
	{
		printf("\r\nSHT35\r\n");
		Delay_ms(1000);
		//iic_Test();
		SHT35_init();

	} 

}



/*********************************************END OF FILE**********************/

