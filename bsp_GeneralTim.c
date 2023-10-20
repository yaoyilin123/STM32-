#include "bsp_GeneralTim.h"

//采用TIM3的CH3通道 完成呼吸灯  PB0
static void GeneralTim_GPIO_Config(void)
{
	//定时器用到的 GPIO 初始化 
	//输出通道的初始化

	GPIO_InitTypeDef GPIO_InitStruct1;
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitStruct1.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct1.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStruct1.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct1);
}


static void GeneralTim_Mode_Config(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //务必打开时钟
		//定义用到的时基
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_TimeBaseStructure.TIM_Prescaler = (720-1);  //分频系数
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = (100-1);
	TIM_TimeBaseStructure.TIM_ClockDivision =  TIM_CKD_DIV1; //时钟分频系数，配置死区时候需要用到
 	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;  //未用到
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	
	//定时器输出比较结构体初始化
	TIM_OCInitTypeDef TIM_OCInitStructure;
  TIM_OCStructInit(&TIM_OCInitStructure); 
	TIM_OCInitStructure.TIM_OCMode =TIM_OCMode_PWM1;  //设置为PWM1模式，CNT<CRR时高电平，CNT>CRR时低电平。
	TIM_OCInitStructure.TIM_OutputState =  TIM_OutputState_Enable ;  //输出使能
	TIM_OCInitStructure.TIM_Pulse = 100;  				//设置CRR值，决定占空比
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  //输出通道极性配置
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;  //输出通道空闲状态配置
	TIM_OC3Init(TIM3, &TIM_OCInitStructure); //对应的是CH3通道三 所以选择OC3Init

	// 使能计数器
	TIM_Cmd(TIM3, ENABLE);	

}

void GeneralTim_Config(void)
{
	GeneralTim_GPIO_Config();
	GeneralTim_Mode_Config();
}
