/**
  ******************************************************************************
  * @file    bsp_i2c_ee.c
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   用gpio模拟i2c总线, 适用于STM32系列CPU。该模块不包括应用层命令帧，仅包括I2C总线基本操作函数。
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火  STM32 F407 开发板  
  *
  ******************************************************************************
  */ 

/*
	应用说明：
	在访问I2C设备前，请先调用 i2c_CheckDevice() 检测I2C设备是否正常，该函数会配置GPIO

*/
#include "iic.h"
#include "bsp_sys.h"
#include "./usart/bsp_debug_usart.h"

//u8 humiture_buff1[20];
//float humiture[4];

static void i2c_CfgGpio(void);


/*
*********************************************************************************************************
*	函 数 名: i2c_Delay
*	功能说明: I2C总线位延迟，最快400KHz
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
static void i2c_Delay(void)
{
	uint8_t i;

	/*　
		可用逻辑分析仪测量I2C通讯时的频率
    工作条件：CPU主频168MHz ，MDK编译环境，1级优化
  
		经测试，循环次数为20~250时都能通讯正常

	*/
	for (i = 0; i < 40; i++);
}

/*
*********************************************************************************************************
*	函 数 名: i2c_Start
*	功能说明: CPU发起I2C总线启动信号
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void i2c_Start(void)
{
	/* 当SCL高电平时，SDA出现一个下跳沿表示I2C总线启动信号 */
	EEPROM_I2C_SDA_1();
	EEPROM_I2C_SCL_1();
	i2c_Delay();
	EEPROM_I2C_SDA_0();
	i2c_Delay();
	EEPROM_I2C_SCL_0();
	i2c_Delay();
}

/*
*********************************************************************************************************
*	函 数 名: i2c_Start
*	功能说明: CPU发起I2C总线停止信号
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void i2c_Stop(void)
{
	/* 当SCL高电平时，SDA出现一个上跳沿表示I2C总线停止信号 */
	EEPROM_I2C_SDA_0();
	EEPROM_I2C_SCL_1();
	i2c_Delay();
	EEPROM_I2C_SDA_1();
}

/*
*********************************************************************************************************
*	函 数 名: i2c_SendByte
*	功能说明: CPU向I2C总线设备发送8bit数据
*	形    参：_ucByte ： 等待发送的字节
*	返 回 值: 无
*********************************************************************************************************
*/
void i2c_SendByte(uint8_t _ucByte)
{
	uint8_t i;

	/* 先发送字节的高位bit7 */
	for (i = 0; i < 8; i++)
	{		
		if (_ucByte & 0x80)
		{
			EEPROM_I2C_SDA_1();
		}
		else
		{
			EEPROM_I2C_SDA_0();
		}
		i2c_Delay();
		EEPROM_I2C_SCL_1();
		i2c_Delay();	
		EEPROM_I2C_SCL_0();
		if (i == 7)
		{
			 EEPROM_I2C_SDA_1(); // 释放总线
		}
		_ucByte <<= 1;	/* 左移一个bit */
		i2c_Delay();
	}
}

/*
*********************************************************************************************************
*	函 数 名: i2c_ReadByte
*	功能说明: CPU从I2C总线设备读取8bit数据
*	形    参：无
*	返 回 值: 读到的数据
*********************************************************************************************************
*/
uint8_t i2c_ReadByte(void)
{
	uint8_t i;
	uint8_t value;

	/* 读到第1个bit为数据的bit7 */
	value = 0;
	for (i = 0; i < 8; i++)
	{
		value <<= 1;
		EEPROM_I2C_SCL_1();
		i2c_Delay();
		if (EEPROM_I2C_SDA_READ())
		{
			value++;
		}
		EEPROM_I2C_SCL_0();
		i2c_Delay();
	}
	return value;
}

/*
*********************************************************************************************************
*	函 数 名: i2c_WaitAck
*	功能说明: CPU产生一个时钟，并读取器件的ACK应答信号
*	形    参：无
*	返 回 值: 返回0表示正确应答，1表示无器件响应
*********************************************************************************************************
*/
uint8_t i2c_WaitAck(void)
{
	uint8_t re;

	EEPROM_I2C_SDA_1();	/* CPU释放SDA总线 */
	i2c_Delay();
	EEPROM_I2C_SCL_1();	/* CPU驱动SCL = 1, 此时器件会返回ACK应答 */
	i2c_Delay();
	if (EEPROM_I2C_SDA_READ())	/* CPU读取SDA口线状态 */
	{
		re = 1;
	}
	else
	{
		re = 0;
	}
	EEPROM_I2C_SCL_0();
	i2c_Delay();
	return re;
}

/*
*********************************************************************************************************
*	函 数 名: i2c_Ack
*	功能说明: CPU产生一个ACK信号
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void i2c_Ack(void)
{
	EEPROM_I2C_SDA_0();	/* CPU驱动SDA = 0 */
	i2c_Delay();
	EEPROM_I2C_SCL_1();	/* CPU产生1个时钟 */
	i2c_Delay();
	EEPROM_I2C_SCL_0();
	i2c_Delay();
	EEPROM_I2C_SDA_1();	/* CPU释放SDA总线 */
}

/*
*********************************************************************************************************
*	函 数 名: i2c_NAck
*	功能说明: CPU产生1个NACK信号
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void i2c_NAck(void)
{
	EEPROM_I2C_SDA_1();	/* CPU驱动SDA = 1 */
	i2c_Delay();
	EEPROM_I2C_SCL_1();	/* CPU产生1个时钟 */
	i2c_Delay();
	EEPROM_I2C_SCL_0();
	i2c_Delay();	
}

/*
*********************************************************************************************************
*	函 数 名: i2c_CfgGpio
*	功能说明: 配置I2C总线的GPIO，采用模拟IO的方式实现
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
static void i2c_CfgGpio(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(EEPROM_I2C_GPIO_CLK, ENABLE);	/* 打开GPIO时钟 */

	GPIO_InitStructure.GPIO_Pin = EEPROM_I2C_SCL_PIN | EEPROM_I2C_SDA_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;  	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;  	/* 开漏输出 */
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(EEPROM_I2C_GPIO_PORT, &GPIO_InitStructure);

	/* 给一个停止信号, 复位I2C总线上的所有设备到待机模式 */
	i2c_Stop();
}

/*
*********************************************************************************************************
*	函 数 名: i2c_CheckDevice
*	功能说明: 检测I2C总线设备，CPU向发送设备地址，然后读取设备应答来判断该设备是否存在
*	形    参：_Address：设备的I2C总线地址
*	返 回 值: 返回值 0 表示正确， 返回1表示未探测到
*********************************************************************************************************
*/
uint8_t i2c_CheckDevice(uint8_t _Address)
{
	uint8_t ucAck;

	i2c_CfgGpio();		/* 配置GPIO */

	
	i2c_Start();		/* 发送启动信号 */

	/* 发送设备地址+读写控制bit（0 = w， 1 = r) bit7 先传 */
	i2c_SendByte(_Address | EEPROM_I2C_WR);
	ucAck = i2c_WaitAck();	/* 检测设备的ACK应答 */

	i2c_Stop();			/* 发送停止信号 */

	return ucAck;
}

/*
*********************************************************************************************************
*	函 数 名: iic_Init
*	功能说明: 封装模拟iic的GPIO口设置,用于iic初始化操作
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void iic_Init(void)
{
	i2c_CfgGpio();
}

/*
*********************************************************************************************************
*	函 数 名: iic_Test
*	功能说明: 测试iic与SHT35的通讯
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
//void iic_Test(void)
//{
//	u16 tem,hum;
//	u16 buff[6];
//	float Temperature=0;
//	float Humidity=0;

//	i2c_CfgGpio();		/* 配置GPIO */

//	i2c_Start();		/* 发送启动信号 */

//	i2c_SendByte(0x44<<1|EEPROM_I2C_WR);    /* CPU向I2C总线设备发送 */
//	i2c_WaitAck();
//	i2c_SendByte(0x2C);
//	i2c_WaitAck();
//	i2c_SendByte(0x06);
//	i2c_WaitAck();

//	i2c_Stop();	
//	Delay_ms(50);

//	i2c_Start();		/* 发送启动信号 */
//	i2c_SendByte(0x44<<1|EEPROM_I2C_RD);    /* CPU向I2C总线设备发送 */
//	if (i2c_WaitAck()==0)
//	{
//		buff[0]=i2c_ReadByte();
//		i2c_Ack();
//		buff[1]=i2c_ReadByte();
//		i2c_Ack();
//		buff[2]=i2c_ReadByte();
//		i2c_Ack();
//		buff[3]=i2c_ReadByte();
//		i2c_Ack();
//		buff[4]=i2c_ReadByte();
//		i2c_Ack();
//		buff[5]=i2c_ReadByte();
//		i2c_NAck();
//		i2c_Stop();
//	}

//	tem = ((buff[0]<<8) | buff[1]);//温度拼接
//	hum = ((buff[3]<<8) | buff[4]);//湿度拼接
//	
//	/*转换实际温度*/
//	Temperature= (175.0*(float)tem/65535.0-45.0) ;// T = -45 + 175 * tem / (2^16-1)
//	Humidity= (100.0*(float)hum/65535.0);// RH = hum*100 / (2^16-1)
//	
//	if((Temperature>=-20)&&(Temperature<=125)&&(Humidity>=0)&&(Humidity<=100))//过滤错误数据
//	{
//		humiture[0]=Temperature;
//		humiture[2]=Humidity;
//		sprintf(humiture_buff1,"{\"Temp\":%.2f,\"Hum\":%.2f}",Temperature,Humidity);//111.01*C 100.01%（保留2位小数）
//	}
//	printf("%s\n",humiture_buff1);
//	hum=0;
//	tem=0;

//}
