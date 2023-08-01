#ifndef __BSP_WIFI_USART_H__
#define __BSP_WIFI_USART_H__

#include "stm32f4xx.h"


#include "stm32f4xx.h"



/******************************* 宏定义 ***************************/
#define            macNVIC_PriorityGroup_x                     NVIC_PriorityGroup_2



/********************************** 函数声明 ***************************************/
void USART_printf( USART_TypeDef * USARTx, unsigned char * Data, ... );
void Usart_SendStringWifi( USART_TypeDef * USARTx, char *str);



#endif /* __COMMON_H */
