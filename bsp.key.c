#include "bsp_key.h"


void KEY1_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	RCC_APB2PeriphClockCmd(KEY1_GPIO_CLK,ENABLE);
	GPIO_InitStruct.GPIO_Pin = KEY1_GPIO_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING; //配置浮空输入模式,原理图上可见，处于外部下拉

  GPIO_Init(KEY1_GPIO_PORT, &GPIO_InitStruct);
}


void KEY2_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	RCC_APB2PeriphClockCmd(KEY2_GPIO_CLK,ENABLE);
	GPIO_InitStruct.GPIO_Pin = KEY2_GPIO_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING; //配置浮空输入模式,原理图上可见，处于外部下拉

  GPIO_Init(KEY2_GPIO_PORT, &GPIO_InitStruct);
}  

uint16_t Key_scan(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
	if(GPIO_ReadInputDataBit( GPIOx, GPIO_Pin)== KEY_ON)   //函数用于读取IDR上的数据
	{
		while(GPIO_ReadInputDataBit( GPIOx, GPIO_Pin))
		{}	
			return KEY_ON;
	}
	else
	{
		return KEY_OFF;
	}
}
