#ifndef __BSP_RELAY_H__
#define __BSP_RELAY_H__

#include "stm32f4xx.h"

#define RELAY1_GPIO_PORT    GPIOB
#define RELAY1_GPIO_CLK     RCC_AHB1Periph_GPIOB
#define RELAY1_GPIO_PIN     GPIO_Pin_12

/* 直接操作寄存器控制IO*/
#define	digitalHi(p,i)		 {p->BSRRL=i;}	 //输出为高电平		
#define digitalLo(p,i)		 {p->BSRRH=i;}	 //输出低电平
#define digitalToggle(p,i) {p->ODR ^=i;} //输出反转状态

/* 定义控制IO的宏 */
#define RELAY1_TOGGLE		    digitalToggle(RELAY1_GPIO_PORT,RELAY1_GPIO_PIN)
#define RELAY1_ON		        digitalHi(RELAY1_GPIO_PORT,RELAY1_GPIO_PIN)
#define RELAY1_OFF			    digitalLo(RELAY1_GPIO_PORT,RELAY1_GPIO_PIN)

void RELAY_GPIO_Config(void);
void Relay_Init(void);

#endif

