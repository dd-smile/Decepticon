#include "stm32f4xx.h"
#include "sht35.h"
#include "iic.h"
#include "bsp_sys.h"
#include "./usart/bsp_debug_usart.h"


u8 humiture_buff1[20];
float humiture[4];

void SHT35_init(void)
{
    u16 tem,hum;
	u16 buff[6];
	float Temperature=0;
	float Humidity=0;

	iic_Init();		/* 配置GPIO */

	i2c_Start();		/* 发送启动信号 */

	i2c_SendByte(0x44<<1|EEPROM_I2C_WR);    /* CPU向I2C总线设备发送 */
	i2c_WaitAck();
	i2c_SendByte(0x2C);
	i2c_WaitAck();
	i2c_SendByte(0x06);
	i2c_WaitAck();

	i2c_Stop();	
	Delay_ms(50);

	i2c_Start();		/* 发送启动信号 */
	i2c_SendByte(0x44<<1|EEPROM_I2C_RD);    /* CPU向I2C总线设备发送 */
	if (i2c_WaitAck()==0)
	{
		buff[0]=i2c_ReadByte();
		i2c_Ack();
		buff[1]=i2c_ReadByte();
		i2c_Ack();
		buff[2]=i2c_ReadByte();
		i2c_Ack();
		buff[3]=i2c_ReadByte();
		i2c_Ack();
		buff[4]=i2c_ReadByte();
		i2c_Ack();
		buff[5]=i2c_ReadByte();
		i2c_NAck();
		i2c_Stop();
	}

	tem = ((buff[0]<<8) | buff[1]);//温度拼接
	hum = ((buff[3]<<8) | buff[4]);//湿度拼接
	
	/*转换实际温度*/
	Temperature= (175.0*(float)tem/65535.0-45.0) ;// T = -45 + 175 * tem / (2^16-1)
	Humidity= (100.0*(float)hum/65535.0);// RH = hum*100 / (2^16-1)
	
	if((Temperature>=-20)&&(Temperature<=125)&&(Humidity>=0)&&(Humidity<=100))//过滤错误数据
	{
		humiture[0]=Temperature;
		humiture[2]=Humidity;
		sprintf(humiture_buff1,"{\"Temp\":%.2f,\"Hum\":%.2f}",Temperature,Humidity);//111.01*C 100.01%（保留2位小数）
	}
	printf("%s\n",humiture_buff1);
	hum=0;
	tem=0;
}
