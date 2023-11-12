#include "bsp_adc_multchannel.h"

//选用ADC1_10-15通道  PC0-PC5
uint16_t ADC_MultChannel_DMA_Data[6]={0,0,0,0,0,0};
static void ADC_GPIO_Config(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct;
	//ADC采用模拟输入
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 |GPIO_Pin_1| GPIO_Pin_2|
														 GPIO_Pin_3 |GPIO_Pin_4|GPIO_Pin_5 ;
  GPIO_Init(GPIOC, &GPIO_InitStruct);
	
}

//配置模式时注意使能的顺序，可能会影响结果
static void ADC_MODE_Config(void)
{
	//DMA时钟
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	
	//DMA初始化
	DMA_InitTypeDef	DMA_InitStructure;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(ADC1->DR);
	//从DR寄存器传到SRAM中的一个数组存储起来
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)ADC_MultChannel_DMA_Data;
	DMA_InitStructure.DMA_DIR =DMA_DIR_PeripheralSRC;
	//BufferSize 和 Channel 数量相同
	DMA_InitStructure.DMA_BufferSize = 6;
	//地址需要递增，这样才能依次对应数组中的位置
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_PeripheralDataSize =DMA_PeripheralDataSize_HalfWord ;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel1 ,&DMA_InitStructure);
	
	//DMA使能
	DMA_Cmd(DMA1_Channel1,ENABLE);
	
	//打开ADC1的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	
	//初始化对应结构体
	ADC_InitTypeDef	ADC_InitStructure;
	//连续转换，这样能持续收集当前状态
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	//软件触发
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_NbrOfChannel =6;
	//多个通道，需要扫描转换
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_Init(ADC1,&ADC_InitStructure);

	//配置ADDCLK
	RCC_ADCCLKConfig(RCC_PCLK2_Div8);
	
	//规则通道配置
	ADC_RegularChannelConfig(ADC1,ADC_Channel_10,1,ADC_SampleTime_41Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_11,2,ADC_SampleTime_41Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_12,3,ADC_SampleTime_41Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_13,4,ADC_SampleTime_41Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_14,5,ADC_SampleTime_41Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_15,6,ADC_SampleTime_41Cycles5);

	//DMA请求
	ADC_DMACmd(ADC1,ENABLE);

	//使能ADC
	ADC_Cmd(ADC1,ENABLE);	
	
	//触发之前校准
	ADC_StartCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1)==SET);
	
	//软件触发
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);
}



void ADC_MultChannel_Config(void)
{
	ADC_GPIO_Config();
	ADC_MODE_Config();
}


