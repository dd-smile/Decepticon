#ifndef __BSP_WIFI_USART_H__
#define __BSP_WIFI_USART_H__

#include "stm32f4xx.h"

/*****************宏定义******************/
#define    macNVIC_PriorityGroup_x          NVIC_PriorityGroup_2

void USART_printf(USART_TypeDef* USARTx,char* Data, ...);


#endif /* __BSP_WIFI_USART_H__ */
