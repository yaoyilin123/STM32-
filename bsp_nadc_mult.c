#include "bsp_nadc_mult.h"

//双ADC模式 各双通道
//ADC1_10.11通道 对应PC0 PC1
//ADC2_12.14通道 对应PC2 PC4

uint32_t ADCx_MultChannel_DMA_Data[2];
static void ADCx_MultChannel_GPIO_Config(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 |GPIO_Pin_1|
														 GPIO_Pin_2|GPIO_Pin_4;
  GPIO_Init(GPIOC, &GPIO_InitStruct);
}

static void ADCx_MultChannel_MODE_Config(void)
{
	//DMA时钟
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	//DMA初始化
	DMA_InitTypeDef	DMA_InitStructure;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(ADC1->DR);
	//从DR寄存器传到SRAM中的一个数组存储起来
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)ADCx_MultChannel_DMA_Data;
	DMA_InitStructure.DMA_DIR =DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = 2;
	//地址需要递增，这样才能依次对应数组中的位置
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
	DMA_InitStructure.DMA_PeripheralDataSize =DMA_PeripheralDataSize_Word ;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel1 ,&DMA_InitStructure);
	
	//DMA使能
	DMA_Cmd(DMA1_Channel1,ENABLE);
	
	//打开ADC时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2,ENABLE);
	
	//ADC1初始化
	ADC_InitTypeDef	ADC_InitStructure;
	ADC_InitStructure.ADC_Mode = ADC_Mode_RegSimult;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;

	ADC_InitStructure.ADC_NbrOfChannel =2;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_Init(ADC1,&ADC_InitStructure);
	ADC_Cmd(ADC1,ENABLE);
	//规则通道的配置
	ADC_RegularChannelConfig(ADC1,ADC_Channel_10,1,ADC_SampleTime_41Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_11,2,ADC_SampleTime_41Cycles5);
	//ADCDMA请求
	ADC_DMACmd(ADC1,ENABLE);	
	//ADCCLK
	RCC_ADCCLKConfig(RCC_PCLK2_Div8);
	
	//ADC2初始化
	ADC_Init(ADC2,&ADC_InitStructure);
	RCC_ADCCLKConfig(RCC_PCLK2_Div8);
	ADC_RegularChannelConfig(ADC2,ADC_Channel_12,1,ADC_SampleTime_41Cycles5);
	ADC_RegularChannelConfig(ADC2,ADC_Channel_14,2,ADC_SampleTime_41Cycles5);
	ADC_Cmd(ADC2,ENABLE);

	
	//校准
	ADC_StartCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1)==SET);
	ADC_StartCalibration(ADC2);
	while(ADC_GetCalibrationStatus(ADC2)==SET);
	
	//触发
	//ADC2由ADC1触发
	ADC_ExternalTrigConvCmd(ADC2,ENABLE); //adc2的触发必须写在1的前面*******
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);

	
}

void ADCx_MultChannel_Init(void)
{
	ADCx_MultChannel_GPIO_Config();
	ADCx_MultChannel_MODE_Config();
}



