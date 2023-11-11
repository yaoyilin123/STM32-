#include "bsp_adc.h"

//选用ADC1-11通道  PC1

static void ADC_GPIO_Config(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;
	//ADC采用模拟输入
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOC, &GPIO_InitStruct);
}

//配置模式时注意使能的顺序，可能会影响结果
static void ADC_MODE_Config(void)
{
	//打开ADC1的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	//初始化对应结构体
	ADC_InitTypeDef	ADC_InitStructure;
	//连续转换，这样能持续收集当前状态
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	//软件触发
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	//只使用一个通道，选用独立模式
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_NbrOfChannel =1;
	//只有一个通道，不用扫描转换
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_Init(ADC1,&ADC_InitStructure);

	//配置ADDCLK
	RCC_ADCCLKConfig(RCC_PCLK2_Div8);
	
	//中断使能，选用规则通道转换结束标志位
	ADC_ITConfig(ADC1,ADC_IT_EOC,ENABLE);
	//使能ADC
	ADC_Cmd(ADC1,ENABLE);	
	
	//规则通道配置
	ADC_RegularChannelConfig(ADC1,ADC_Channel_11,1,ADC_SampleTime_41Cycles5);
	
	//触发之前校准
	ADC_StartCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1)==RESET);
	
	//软件触发
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);
}


static void NVIC_Config(void)
{
	NVIC_InitTypeDef	NVIC_InitStucture;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStucture.NVIC_IRQChannel = ADC1_2_IRQn;
	NVIC_InitStucture.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStucture.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStucture.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStucture);
	
}

void ADC_Config(void)
{
	NVIC_Config();
	ADC_GPIO_Config();
	ADC_MODE_Config();
}


