#include "PWM.h"
#include "bsp_systick.h"

extern uint16_t MotorPsc;

static void NVIC_Config(void)
{
	NVIC_InitTypeDef	NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_Init(&NVIC_InitStructure);
}


//使用基本定时器TIM3_CH2  PA7
static void GeneralTim_PWM_Config(void)
{
	//开启时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	
	//时基初始化
	TIM_TimeBaseInitTypeDef	TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	//ARR
	TIM_TimeBaseInitStructure.TIM_Period = (100-1);  
	//PSC
	TIM_TimeBaseInitStructure.TIM_Prescaler = (720-1);
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);
	
	//OC比较初始化
	TIM_OCInitTypeDef	TIM_OCInitStructure;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	//CCR
	TIM_OCInitStructure.TIM_Pulse = 50;
	TIM_OC2Init(TIM3,&TIM_OCInitStructure);
	
	//开启影子寄存器
	TIM_OC2PreloadConfig(TIM3,TIM_OCPreload_Enable);
	
	//配置中断优先级
	NVIC_Config();
	
	//使能计数器
	TIM_Cmd(TIM3,ENABLE);
	
}


//TIM3_CH2对应PA7
static void GeneralTim_GPIO_Config(void) 
{
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
}

static void Motor_Dir(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	GPIO_ResetBits(GPIOA,GPIO_Pin_5);
}


void GeneralTim_Config(void)
{
	GeneralTim_GPIO_Config();
	GeneralTim_PWM_Config();
	Motor_Dir();
} 


//控制电机加速—匀速—减速

void MSD_Move(int step,Accel_TypeDef Accel,Dccel_TypeDef Decel,MotorSpeed_TypeDef Speed)
{   
	int i;
	
	//首先判别正转反转
	if(step > 0)
	{
		Dir_Positive();
	}
	else
	{
		Dir_Reverse();
	}
	 
	//开始加速
	while(1)
	{
		TIM_SetCompare2(TIM3,50);
		switch (Accel)
		{
			case Accel_Fast:		
				for(i=MotorPsc;i>Speed;i-=6)
				{
					TIM_PrescalerConfig(TIM3,i,TIM_PSCReloadMode_Update);
					SysTick_Delay_ms(20);
				}
				break;
			case Accel_Common:
				for(i=MotorPsc;i>Speed;i-=3)
				{
					TIM_PrescalerConfig(TIM3,i,TIM_PSCReloadMode_Update);
					SysTick_Delay_ms(20);
				}
				break;
			case Accel_Slow:
				for(i=MotorPsc;i>Speed;i-=1)
				{
					TIM_PrescalerConfig(TIM3,i,TIM_PSCReloadMode_Update);
					SysTick_Delay_ms(20);
				}
				break;
			default: break;
		}
		
		//达到匀速状态，蜂鸣器响一下(由于延时函数，会影响脉冲数量的准确性,如果需要精确的话，可以删除，也可以换成RGB灯)
		Beep_ON();
		SysTick_Delay_ms(1);
		Beep_OFF();
		
		//进入中断开始计数，走指定的脉冲数
		TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
		
		//检测DIER寄存器的UIE位
		while(TIM3->DIER & (1<<0));
		
		//走完给定的脉冲数，蜂鸣器响一下
		Beep_ON();
		SysTick_Delay_ms(20);
		Beep_OFF();
		
		//开始减速
		switch (Decel)
		{
			case Dccel_Fast:		
				for(;i<MotorPsc;i+=6)  		/* 此时i的大小为speed所对应命令的数值，所以不需要赋值	*/
				{
					TIM_PrescalerConfig(TIM3,i,TIM_PSCReloadMode_Update);
					SysTick_Delay_ms(20);
				}
				break;
			case Dccel_Common:
				for(;i<MotorPsc;i+=3)
				{
					TIM_PrescalerConfig(TIM3,i,TIM_PSCReloadMode_Update);
					SysTick_Delay_ms(20);
				}
				break;
			case Dccel_Slow:
				for(;i<MotorPsc;i+=1)
				{
					TIM_PrescalerConfig(TIM3,i,TIM_PSCReloadMode_Update);
					SysTick_Delay_ms(20);
				}
				break;
			default: break;
		}
		TIM_SetCompare2(TIM3,0);
		SysTick_Delay_ms(1000);
	}

}


void Beep_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
  Beep_OFF();
} 


