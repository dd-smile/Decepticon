#include "bsp_relay.h"
#include "bsp_led.h"
#include "bsp_key.h"

/*
*********************************************************************************************************
*	函 数 名: RELAY_GPIO_Config
*	功能说明: 继电器的GPIO口设置
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void RELAY_GPIO_Config(void)
{		

    GPIO_InitTypeDef GPIO_InitStructure;


    RCC_AHB1PeriphClockCmd ( RELAY1_GPIO_CLK, ENABLE); 
                                                            
    GPIO_InitStructure.GPIO_Pin = RELAY1_GPIO_PIN;	
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;	
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;

    GPIO_Init(RELAY1_GPIO_PORT, &GPIO_InitStructure);	    
                                                    
    GPIO_ResetBits(RELAY1_GPIO_PORT, RELAY1_GPIO_PIN);
}

/*
*********************************************************************************************************
*	函 数 名: Relay_Init
*	功能说明: 继电器初始化,控制继电器1启动
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void Relay_Init(void)
{
    if ( Key_Scan(KEY1_GPIO_PORT,KEY1_PIN) == KEY_ON  ) 
		{
            RELAY1_TOGGLE;
        }

		if ( Key_Scan(KEY2_GPIO_PORT,KEY2_PIN) == KEY_ON  ) 
		{
			/*LED2反转*/
			LED2_TOGGLE;
		}

}