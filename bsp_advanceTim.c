#include "bsp_advanceTim.h"

//定时器选择TIM1  输出通道PA8  互补输出PB13   死区PB12
static void AdvanceTim_GPIO_Config(void)
{
	//定时器用到的 GPIO 初始化 
	//输出通道的初始化
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin = TIM1_CH_GPIO_PIN;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(TIM1_CH_GPIO_PORT, &GPIO_InitStruct);
	//互补输出的初始化
	GPIO_InitTypeDef GPIO_InitStruct1;
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitStruct1.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct1.GPIO_Pin = TIM1_CHN_GPIO_PIN;
	GPIO_InitStruct1.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(TIM1_CHN_GPIO_PORT, &GPIO_InitStruct1);
	//死区的初始化
	GPIO_InitTypeDef GPIO_InitStruct2;
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitStruct2.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct2.GPIO_Pin = TIM1_CHBK_GPIO_PIN;
	GPIO_InitStruct2.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(TIM1_CHBK_GPIO_PORT, &GPIO_InitStruct2);
	//BKIN默认低电平
	GPIO_ResetBits(GPIOB,GPIO_Pin_12);
}


static void AdvanceTim_TimeBase_Config(void)
{
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_TIM1,ENABLE); //务必打开时钟
		//定义用到的时基
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_TimeBaseStructure.TIM_Prescaler = (9-1);  //分频系数
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = (8-1);
	TIM_TimeBaseStructure.TIM_ClockDivision =  TIM_CKD_DIV1; //时钟分频系数，配置死区时候需要用到
 	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;  //未用到
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
	

}

static void OCInit_Config(void)
{
	//定时器输出比较结构体初始化
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCInitStructure.TIM_OCMode =TIM_OCMode_PWM1;  //设置为PWM1模式，CNT<CRR时高电平，CNT>CRR时低电平。
	TIM_OCInitStructure.TIM_OutputState =  TIM_OutputState_Enable ;  //输出使能
	TIM_OCInitStructure.TIM_OutputNState=  TIM_OutputNState_Enable ; //互补输出使能
	TIM_OCInitStructure.TIM_Pulse = 4;  				//设置CRR值，决定占空比
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  //输出通道极性配置
	TIM_OCInitStructure.TIM_OCNPolarity= TIM_OCNPolarity_High; //互补通道极性配置
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;  //输出通道空闲状态配置
	TIM_OCInitStructure.TIM_OCNIdleState =TIM_OCNIdleState_Reset;//互补通道空闲状态配置
	TIM_OC1Init(TIM1, &TIM_OCInitStructure); 
	TIM_OC1PreloadConfig(TIM1,TIM_OCPreload_Enable); //预装载寄存器使能，（影子寄存器）
	TIM_BDTRInitTypeDef TIM_BDTRInitStructure;
	TIM_BDTRInitStructure.TIM_OSSRState = TIM_OSSRState_Enable;
	TIM_BDTRInitStructure.TIM_OSSIState = TIM_OSSIState_Enable;
	TIM_BDTRInitStructure.TIM_LOCKLevel = TIM_LOCKLevel_1;
 // 输出比较信号死区时间配置，具体如何计算可参考 BDTR:UTG[7:0] 的描述
 // 这里配置的死区时间为 152ns
  TIM_BDTRInitStructure.TIM_DeadTime = 11;
  TIM_BDTRInitStructure.TIM_Break = TIM_Break_Enable;
 // 当 BKIN 引脚检测到高电平的时候，输出比较信号被禁止，就好像是刹车一样
  TIM_BDTRInitStructure.TIM_BreakPolarity = TIM_BreakPolarity_High;
  TIM_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Enable;
  TIM_BDTRConfig(TIM1, &TIM_BDTRInitStructure);

 // 使能计数器
  TIM_Cmd(TIM1, ENABLE);
  // 主输出使能，当使用的是通用定时器时，这句不需要
  TIM_CtrlPWMOutputs(TIM1, ENABLE);
}


void AdvanceTim_Config(void)
{
	AdvanceTim_GPIO_Config();
	AdvanceTim_TimeBase_Config();
	OCInit_Config();
}
