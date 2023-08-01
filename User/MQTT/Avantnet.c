//单片机头文件
#include "stm32f4xx.h"

//网络设备
#include "esp8266.h"

//协议文件
#include "avantnet.h"
#include "mqttkit.h"

//硬件驱动
#include "./usart/bsp_debug_usart.h"
#include "./dwt_delay/core_delay.h"
#include "bsp_led.h"

//C库
#include <string.h>
#include <stdio.h>
#include "cJSON.h"


#define PROID		"admin"

#define AUTH_INFO	"public"

#define DEVID		"avant7815"

uint8_t led_switch = 0xff;    //定义led开关初始化值


//==========================================================
//	函数名称：	AvantNet_DevLink
//  
//	函数功能：	与mqtt服务器创建连接
//
//	入口参数：	无
//
//	返回参数：	1-成功	0-失败
//
//	说明：		与AvantNet平台建立连接
//==========================================================
_Bool AvantNet_DevLink(void)
{
	
	MQTT_PACKET_STRUCTURE mqttPacket = {NULL, 0, 0, 0};					//协议包

	unsigned char *dataPtr;
	
	_Bool status = 1;
	
	UsartPrintf(DEBUG_USART, "AvantNet_DevLink\r\n"
							"PROID: %s,	AUIF: %s,	DEVID:%s\r\n"
                        , PROID, AUTH_INFO, DEVID);
	
	if(MQTT_PacketConnect(PROID, AUTH_INFO, DEVID, 256, 0, MQTT_QOS_LEVEL0, NULL, NULL, 0, &mqttPacket) == 0)
	{
		while( ! ESP8266_UnvarnishSend () );
		ESP8266_SendData(mqttPacket._data);			//上传平台
		
		dataPtr = ESP8266_GetIPD(250);									//等待平台响应
		if(dataPtr != NULL)
		{
			if(MQTT_UnPacketRecv(dataPtr) == MQTT_PKT_CONNACK)
			{
				switch(MQTT_UnPacketConnectAck(dataPtr))
				{
					case 0:UsartPrintf(DEBUG_USART, "Tips:	连接成功\r\n");status = 0;break;
					
					case 1:UsartPrintf(DEBUG_USART, "WARN:	连接失败：协议错误\r\n");break;
					case 2:UsartPrintf(DEBUG_USART, "WARN:	连接失败：非法的clientid\r\n");break;
					case 3:UsartPrintf(DEBUG_USART, "WARN:	连接失败：服务器失败\r\n");break;
					case 4:UsartPrintf(DEBUG_USART, "WARN:	连接失败：用户名或密码错误\r\n");break;
					case 5:UsartPrintf(DEBUG_USART, "WARN:	连接失败：非法链接\r\n");break;
					
					default:UsartPrintf(DEBUG_USART, "ERR:	连接失败：未知错误\r\n");break;
				}
			}
		}
		
		MQTT_DeleteBuffer(&mqttPacket);								//删包
	}
	else
		//UsartPrintf(DEBUG_USART, "WARN:	MQTT_PacketConnect Failed\r\n");
	
	return status;
	
}

//==========================================================
//	函数名称：	AvantNet_Subscribe
//
//	函数功能：	订阅
//
//	入口参数：	topics：订阅的topic
//				topic_cnt：topic个数
//
//	返回参数：	SEND_TYPE_OK-成功	SEND_TYPE_SUBSCRIBE-需要重发
//
//	说明：		
//==========================================================
void AvantNet_Subscribe(const char *topics[], unsigned char topic_cnt)
{
	
	unsigned char i = 0;
	
	MQTT_PACKET_STRUCTURE mqttPacket = {NULL, 0, 0, 0};							//协议包
	
	for(; i < topic_cnt; i++)
		UsartPrintf(DEBUG_USART, "Subscribe Topic: %s\r\n", topics[i]);
	
	if(MQTT_PacketSubscribe(MQTT_SUBSCRIBE_ID, MQTT_QOS_LEVEL0, topics, topic_cnt, &mqttPacket) == 0)
	{
		//ESP8266_SendData(mqttPacket._data, mqttPacket._len);					//向平台发送订阅请求
		
		MQTT_DeleteBuffer(&mqttPacket);											//删包
	}

}

//==========================================================
//	函数名称：	AvantNet_Publish
//
//	函数功能：	发布消息
//
//	入口参数：	topic：发布的主题
//				msg：消息内容
//
//	返回参数：	SEND_TYPE_OK-成功	SEND_TYPE_PUBLISH-需要重送
//
//	说明：		
//==========================================================
void AvantNet_Publish(const char *topic, const char *msg)
{

	MQTT_PACKET_STRUCTURE mqttPacket = {NULL, 0, 0, 0};							//协议包
	
	UsartPrintf(DEBUG_USART, "Publish Topic: %s, Msg: %s\r\n", topic, msg);
	
	if(MQTT_PacketPublish(MQTT_PUBLISH_ID, topic, msg, strlen(msg), MQTT_QOS_LEVEL1, 0, 1, &mqttPacket) == 0)
	{
		ESP8266_SendData(mqttPacket._data);					//向平台发送订阅请求
		//ESP8266_SendString(mqttPacket._data);
		
		MQTT_DeleteBuffer(&mqttPacket);											//删包
	}

}

//==========================================================
//	函数名称：	AvantNet_RevPro
//
//	函数功能：	平台返回数据检测
//
//	入口参数：	dataPtr：平台返回的数据
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void AvantNet_RevPro(unsigned char *cmd)
{
	
	MQTT_PACKET_STRUCTURE mqttPacket = {NULL, 0, 0, 0};								//协议包
	
	char *req_payload = NULL;
	char *cmdid_topic = NULL;
	char *led_topic = "/mytopic/sub";
	
	unsigned short topic_len = 0;
	unsigned short req_len = 0;
	
	unsigned char type = 0;
	unsigned char qos = 0;
	static unsigned short pkt_id = 0;
	
	short result = 0;

	char *dataPtr = NULL;
	char numBuf[10];
	int num = 0;
	
	cJSON *json, *json_value;
	
	type = MQTT_UnPacketRecv(cmd);
	switch(type)
	{
		case MQTT_PKT_CMD:															//命令下发
			
			result = MQTT_UnPacketCmd(cmd, &cmdid_topic, &req_payload, &req_len);	//解出topic和消息体
			if(result == 0)
			{
				UsartPrintf(DEBUG_USART, "cmdid: %s, req: %s, req_len: %d\r\n", cmdid_topic, req_payload, req_len);
				
				if(MQTT_PacketCmdResp(cmdid_topic, req_payload, &mqttPacket) == 0)	//命令回复组包
				{
					UsartPrintf(DEBUG_USART, "Tips:	Send CmdResp\r\n");
					
					//ESP8266_SendData(mqttPacket._data, mqttPacket._len);			//回复命令
					MQTT_DeleteBuffer(&mqttPacket);									//删包
				}
			}
		
		break;
			
		case MQTT_PKT_PUBLISH:														//接收的Publish消息
		
			result = MQTT_UnPacketPublish(cmd, &cmdid_topic, &topic_len, &req_payload, &req_len, &qos, &pkt_id);
			if(result == 0)
			{
				UsartPrintf(DEBUG_USART, "topic: %s, topic_len: %d, payload: %s, payload_len: %d\r\n",
																	cmdid_topic, topic_len, req_payload, req_len);
				 
				if((strcmp(cmdid_topic,led_topic) == 0))    //判断是否是led灯开关主题
				{
				    //解析数据包
                    json = cJSON_Parse(req_payload);
                    if(!json)UsartPrintf(DEBUG_USART,"Error before: [%s]\n",cJSON_GetErrorPtr());
                    else
                    {
                        json_value = cJSON_GetObjectItem(json,"LED_SW");
                        UsartPrintf(DEBUG_USART,"json_value = %d\r\n",json_value);
                        if(json_value->valueint)//json_value > 0 且为整型
                        {
                            //打开led
                            LED1_TOGGLE;
                            UsartPrintf(DEBUG_USART,"kkkkkkkkkk json_value = %d\r\n",json_value);
                        }
                        else
                        {
                            //关闭led
                            LED2_TOGGLE;
                            UsartPrintf(DEBUG_USART,"gggggggggg json_value = %d\r\n",json_value);
                        }
                    }
                    //删json包, 防止内存炸
                    cJSON_Delete(json);
                    Delay_ms(500);
                }
			}
		
		break;
			
		case MQTT_PKT_PUBACK:														//发送Publish消息，平台回复的Ack
		
			if(MQTT_UnPacketPublishAck(cmd) == 0)
				UsartPrintf(DEBUG_USART, "Tips:	MQTT Publish Send OK\r\n");
			
		break;
			
		case MQTT_PKT_PUBREC:														//发送Publish消息，平台回复的Rec，设备需回复Rel消息
		
			if(MQTT_UnPacketPublishRec(cmd) == 0)
			{
				UsartPrintf(DEBUG_USART, "Tips:	Rev PublishRec\r\n");
				if(MQTT_PacketPublishRel(MQTT_PUBLISH_ID, &mqttPacket) == 0)
				{
					UsartPrintf(DEBUG_USART, "Tips:	Send PublishRel\r\n");
					//ESP8266_SendData(mqttPacket._data, mqttPacket._len);
					MQTT_DeleteBuffer(&mqttPacket);
				}
			}
		
		break;
			
		case MQTT_PKT_PUBREL:														//收到Publish消息，设备回复Rec后，平台回复的Rel，设备需再回复Comp
			
			if(MQTT_UnPacketPublishRel(cmd, pkt_id) == 0)
			{
				UsartPrintf(DEBUG_USART, "Tips:	Rev PublishRel\r\n");
				if(MQTT_PacketPublishComp(MQTT_PUBLISH_ID, &mqttPacket) == 0)
				{
					UsartPrintf(DEBUG_USART, "Tips:	Send PublishComp\r\n");
					//ESP8266_SendData(mqttPacket._data, mqttPacket._len);
					MQTT_DeleteBuffer(&mqttPacket);
				}
			}
		
		break;
		
		case MQTT_PKT_PUBCOMP:														//发送Publish消息，平台返回Rec，设备回复Rel，平台再返回的Comp
		
			if(MQTT_UnPacketPublishComp(cmd) == 0)
			{
				UsartPrintf(DEBUG_USART, "Tips:	Rev PublishComp\r\n");
			}
		
		break;
			
		case MQTT_PKT_SUBACK:														//发送Subscribe消息的Ack
		
			if(MQTT_UnPacketSubscribe(cmd) == 0)
				UsartPrintf(DEBUG_USART, "Tips:	MQTT Subscribe OK\r\n");
			else
				UsartPrintf(DEBUG_USART, "Tips:	MQTT Subscribe Err\r\n");
		
		break;
			
		case MQTT_PKT_UNSUBACK:														//发送UnSubscribe消息的Ack
		
			if(MQTT_UnPacketUnSubscribe(cmd) == 0)
				UsartPrintf(DEBUG_USART, "Tips:	MQTT UnSubscribe OK\r\n");
			else
				UsartPrintf(DEBUG_USART, "Tips:	MQTT UnSubscribe Err\r\n");
		
		break;
		
		default:
			result = -1;
		break;
	}
	
	//ESP8266_Clear();									//清空缓存
	
	if(result == -1)
		return;
	
	dataPtr = strchr(req_payload, '}');					//搜索'}'

	if(dataPtr != NULL && result != -1)					//如果找到了
	{
		dataPtr++;
		
		while(*dataPtr >= '0' && *dataPtr <= '9')		//判断是否是下发的命令控制数据
		{
			numBuf[num++] = *dataPtr++;
		}
		
		num = atoi((const char *)numBuf);				//转为数值形式
		
	}

	if(type == MQTT_PKT_CMD || type == MQTT_PKT_PUBLISH)
	{
		MQTT_FreeBuffer(cmdid_topic);
		MQTT_FreeBuffer(req_payload);
	}

}
