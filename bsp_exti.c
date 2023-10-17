#include "bsp_exti.h"


static void KEY_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	RCC_APB2PeriphClockCmd(KEY1_GPIO_CLK,ENABLE);
	GPIO_InitStruct.GPIO_Pin = KEY1_GPIO_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	
  GPIO_Init(KEY1_GPIO_PORT, &GPIO_InitStruct);
}



static void NVIC_Config()
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	
}

void EXTI_Config(void)
{
		EXTI_InitTypeDef EXTI_InitStructure;
	//配置中断优先级 NVIC
	NVIC_Config();
	
	//初始化GPIO
	KEY_GPIO_Config();
	
	//初始化AFIO、EXTI
//	AFIO_TypeDef AFIO_InitSturture;
//	AFIO_InitSturture.EXTIC =  ;
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA , GPIO_PinSource0);
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

}


