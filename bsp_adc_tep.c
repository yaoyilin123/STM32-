#include "bsp_adc_tep.h"
uint16_t ADCx_Tep_Data;
//内部温度传感器采用第16通道
void ADCx_Mode_Config(void)
{
	//打开时钟
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	RCC_ADCCLKConfig(RCC_PCLK2_Div8);	
	
	//初始化DMA
	DMA_InitTypeDef	DMA_InitStructure;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(ADC1->DR);
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&ADCx_Tep_Data;
	DMA_InitStructure.DMA_DIR =DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = 1;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_PeripheralDataSize =DMA_PeripheralDataSize_HalfWord ;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel1 ,&DMA_InitStructure);
	
	//使能DMA
	DMA_Cmd(DMA1_Channel1,ENABLE);
	
	//初始化ADC
	ADC_InitTypeDef	ADC_InitStructure;
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;	
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_NbrOfChannel =1;
	ADC_Init(ADC1,&ADC_InitStructure);
	
	//唤醒温度传感器
	ADC_TempSensorVrefintCmd(ENABLE);
	//ADC使能
	ADC_Cmd(ADC1,ENABLE);
	
	//DMA请求
	ADC_DMACmd(ADC1,ENABLE);	
	
	//通道配置
	ADC_RegularChannelConfig(ADC1,ADC_Channel_16,1,ADC_SampleTime_239Cycles5);

	//校准
	ADC_StartCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1)==SET);
	
	//触发
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);
}

