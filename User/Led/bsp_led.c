#include "bsp_led.h"
#include "./systick/bsp_SysTick.h"

/*
*********************************************************************************************************
*	函 数 名: LED_GPIO_Config
*	功能说明: LED灯的GPIO口设置
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void LED_GPIO_Config(void)
{
    /*定义一个GPIO_InitTypeDef类型的结构体*/
    GPIO_InitTypeDef GPIO_InitStructure;

    /*开启LED相关的GPIO外设时钟*/
    RCC_AHB1PeriphClockCmd ( LED1_GPIO_CLK|
                            LED2_GPIO_CLK|
                            LED3_GPIO_CLK, ENABLE);

    /*选择要控制的GPIO引脚*/
    GPIO_InitStructure.GPIO_Pin = LED1_PIN;

    /*设置引脚模式为输出模式*/
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;

    /*设置引脚的输出类型为推挽输出*/
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;

    /*设置引脚为上拉模式*/
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

    /*设置引脚速率为2MHz */
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;

    /*调用库函数，使用上面配置的GPIO_InitStructure初始化GPIO*/
    GPIO_Init(LED1_GPIO_PORT, &GPIO_InitStructure);

    /*选择要控制的GPIO引脚*/
    GPIO_InitStructure.GPIO_Pin = LED2_PIN;
    GPIO_Init(LED2_GPIO_PORT, &GPIO_InitStructure);

    /*选择要控制的GPIO引脚*/
    GPIO_InitStructure.GPIO_Pin = LED3_PIN;
    GPIO_Init(LED3_GPIO_PORT, &GPIO_InitStructure);

    /*关闭RGB灯*/
    LED_RGBOFF;
}

/*
*********************************************************************************************************
*	函 数 名: LED_RGB_Init
*	功能说明: 循环显示RGB流水灯
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void LED_RGB_Init(void)
{
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
