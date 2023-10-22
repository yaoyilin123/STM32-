#include "bsp_led.h"

void LED_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	//配置红灯
	RCC_APB2PeriphClockCmd(LED_R_GPIO_CLK,ENABLE);
	GPIO_InitStruct.GPIO_Pin = LED_R_GPIO_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_Init(LED_R_GPIO_PORT, &GPIO_InitStruct);
	//配置绿灯
	RCC_APB2PeriphClockCmd(LED_G_GPIO_CLK,ENABLE);
	GPIO_InitStruct.GPIO_Pin = LED_G_GPIO_PIN;
  GPIO_Init(LED_G_GPIO_PORT, &GPIO_InitStruct);
	//配置蓝灯
	RCC_APB2PeriphClockCmd(LED_B_GPIO_CLK,ENABLE);
	GPIO_InitStruct.GPIO_Pin = LED_B_GPIO_PIN;
  GPIO_Init(LED_B_GPIO_PORT, &GPIO_InitStruct);
	//初始化时，默认状态灯就是开的
	LED_ALLOFF;
	
}




