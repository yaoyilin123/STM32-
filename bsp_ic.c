#include "bsp_ic.h"

//使用TIM3输出PWM波形，然后用TIM2输入捕获

//TIM3—CHANNEL1  PA6
void OC_Config(void)  
{
	//打开对应时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	//打开对应端口
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,& GPIO_InitStructure);

	//配置时基单元
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_TimeBaseStructure.TIM_ClockDivision= TIM_CKD_DIV1;   //1分频，每遇到一个上升或者下降沿就记录一次
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = 100-1;   //Freq = 72M / 100/Pre +1 
	TIM_TimeBaseStructure.TIM_Prescaler = 720-1 ;  
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);
	
	//配置输出单元
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High ;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 50;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	TIM_OC1Init(TIM3,& TIM_OCInitStructure); //1通道
	
	//计时器使能
	TIM_Cmd(TIM3,ENABLE);
	
}


//TIM4_CHANNEL2 PB7
void IC_Config(void)
{
		//打开对应时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	//打开对应端口
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_Init(GPIOB,& GPIO_InitStructure);

	//配置时基单元
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_TimeBaseStructure.TIM_ClockDivision= TIM_CKD_DIV1;   //1分频，每遇到一个上升或者下降沿就记录一次
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = 65535-1;   //Freq = 72M / 100/Pre +1 
	TIM_TimeBaseStructure.TIM_Prescaler = 720-1 ;  
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);
	
	//配置输入单元
	TIM_ICInitTypeDef TIM_ICInitStructrue;
	TIM_ICInitStructrue.TIM_Channel = TIM_Channel_2 ;  //通道二捕获的是ARR
	TIM_ICInitStructrue.TIM_ICFilter = 0xf;
	TIM_ICInitStructrue.TIM_ICPolarity = TIM_ICPolarity_Rising ;
	TIM_ICInitStructrue.TIM_ICPrescaler = TIM_ICPSC_DIV1 ;
	TIM_ICInitStructrue.TIM_ICSelection = TIM_ICSelection_DirectTI;  //直接通道在CH2输出，另一路在CH1输出
	/*	TIM_ICInitStructrue.TIM_Channel = TIM_Channel_1 ;
	TIM_ICInitStructrue.TIM_ICFilter = 0xf;
	TIM_ICInitStructrue.TIM_ICPolarity = TIM_ICPolarity_Falling ;           //通道一捕获的是CRR
	TIM_ICInitStructrue.TIM_ICPrescaler = TIM_ICPSC_DIV1 ;
	TIM_ICInitStructrue.TIM_ICSelection =TIM_ICSelection_IndirectTI;*/
	
	TIM_PWMIConfig(TIM4, &TIM_ICInitStructrue); //此函数等同于上面注释掉的
	TIM_ICInit(TIM4, &TIM_ICInitStructrue);
	
	//主触发模式
	TIM_SelectInputTrigger(TIM4, TIM_TS_TI2FP2);
	
	//从模式——使计数器归零
	TIM_SelectSlaveMode(TIM4, TIM_SlaveMode_Reset);
	
	//TIM4计时器使能
	TIM_Cmd(TIM4,ENABLE);
}

//在捕获后，将计数器值传到串口 
//USArt1—TX  对应PA9
void Usart1_Config(void) 
{
	//打开串口的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	//打开对应端口
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_Init(GPIOA,& GPIO_InitStructure);
	
	//配置串口结构体
	USART_InitTypeDef USART_InitStructrue;
	USART_InitStructrue.USART_BaudRate = 115200;
	USART_InitStructrue.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructrue.USART_Mode = USART_Mode_Tx;
	USART_InitStructrue.USART_Parity = USART_Parity_No; //校验位
	USART_InitStructrue.USART_StopBits =USART_StopBits_1;
	USART_InitStructrue.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1,&USART_InitStructrue);
	
	//使能串口
	USART_Cmd(USART1, ENABLE);
}

uint16_t IC_Get_Freq(void) 
{
		return 	1000000/(TIM_GetCapture2(TIM4)+1);		//Freq = Fc/N
}

uint16_t IC_Get_Duty(void)
{
		return (TIM_GetCapture1(TIM4)+1)*100 / (TIM_GetCapture2(TIM4)) ;  //  Duty = CRR*100 / ARR %
}

