#include "bsp_tim_ic.h"

//脉宽的输入捕获-利用通用定时器5 TIM5-CH1
//检测按键PA0按下的时间

//创建变量存储标志位用于判别
TIM_InputCapture_TypeInit InputCapture_TypeStructure = {0,0,0,0};

static void TIM_KEY_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStruct);
}	

static void NVIC_TIM_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority= 1;
	NVIC_Init (&NVIC_InitStructure);
}

static void TIM_IC_Config(void)
{
	//打开对应时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);
	
	//配置时基单元
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_TimeBaseStructure.TIM_Prescaler = 72-1;  //72分频  1M 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up ;
	TIM_TimeBaseStructure.TIM_Period = 0xFFFF;
	TIM_TimeBaseStructure.TIM_ClockDivision =TIM_CKD_DIV1 ;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseStructure);
	
	//IC配置
	TIM_ICInitTypeDef TIM_ICInitStructure;
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
	TIM_ICInitStructure.TIM_ICFilter = 0;
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1 ;
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI ;
	TIM_ICInit(TIM5, &TIM_ICInitStructure);
	
	//NVIC优先级的配置
	NVIC_TIM_Config();
	//中断允许
	TIM_ClearFlag(TIM5,TIM_FLAG_Update |TIM_IT_CC1);  //记得开启两个通道！！！
	TIM_ITConfig(TIM5, TIM_IT_Update |TIM_IT_CC1, ENABLE); //采用1通道
	//打开计时器
	TIM_Cmd(TIM5,ENABLE);
	
}

void TIM_IC_exam_Config(void)
{
	TIM_KEY_Config();
	NVIC_TIM_Config();
	TIM_IC_Config();
}


