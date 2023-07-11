//单片机头文件
#include "stm32f4xx.h"
#include "stm32f4xx_it.h"

//板载硬件驱动
#include "./usart/bsp_debug_usart.h"
#include "./systick/bsp_SysTick.h"
#include "bsp_led.h"
#include "bsp_key.h"
#include "bsp_relay.h"

//硬件驱动

//传感器
#include "DHT11/bsp_dht11.h"
#include "sht35.h"
#include "mpu6050.h"

//通信协议
#include "iic.h"
#include "mpu_iic.h"

/* MPU6050数据 */
short Acel[3];
short Gyro[3];
float Temp;

int main(void)
{	

//	Debug_USART_Config();//串口初始化

//	SysTick_Init();//系统定时器初始化
//	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;//启动定时器

	//LED_GPIO_Config();//RGB灯初始化

	//Key_GPIO_Config();//按键初始化

	//RELAY_GPIO_Config();//继电器IO初始化

//	while(1)
//	{
//		//printf("\r\nSHT35\r\n");
//		//Delay_ms(1000);
//		//SHT35_init();   //SHT35串口显示温湿度数据
//		//LED_RGB_Init();   //LED_RGB循环显示流水灯
//		//Relay_Init();//控制继电器启动
//	} 

	/* LED 端口初始化 */
  LED_GPIO_Config();
  
  /*初始化USART1*/
  Debug_USART_Config(); 
	
#ifdef USE_LCD_DISPLAY	
  /*初始化液晶屏*/
	ILI9806G_Init ();         //LCD 初始化
	
	//其中0、3、5、6 模式适合从左至右显示文字，
	//不推荐使用其它模式显示文字	其它模式显示文字会有镜像效果			
	//其中 6 模式为大部分液晶例程的默认显示方向  
  ILI9806G_GramScan ( 6 );

#endif

  /* 配置SysTick定时器和中断 */
  SysTick_Init(); //配置 SysTick 为 1ms 中断一次，在中断里读取传感器数据
  SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; //启动定时器

	//初始化 I2C
	I2cMaster_Init(); 
  //MPU6050初始化
	MPU6050_Init();
  //检测MPU6050
	if( MPU6050ReadID() == 0 )
  {
		printf("\r\n没有检测到MPU6050传感器！\r\n");
		LED_RED;
    
    
		while(1);	//检测不到MPU6050 会红灯亮然后卡死
	}

	printf("\r\n 欢迎使用野火  STM32 F407 开发板。\r\n");		 
	printf("\r\n 这是一个I2C外设(MPU6050)读写测试例程 \r\n");
	
  while(1)
  {
		Delay_ms(1000);
    if( task_readdata_finish ) //task_readdata_finish = 1 表示读取MPU6050数据完成
    {
      
      printf("加速度：%8d%8d%8d",Acel[0],Acel[1],Acel[2]);
      
      printf("    陀螺仪%8d%8d%8d",Gyro[0],Gyro[1],Gyro[2]);
      
      printf("    温度%8.2f\r\n",Temp);
      
      
      task_readdata_finish = 0; // 清零标志位
    }
    
  }


}




/*********************************************END OF FILE**********************/

