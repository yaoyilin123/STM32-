#include "bsp_adc_dma.h"

//选用ADC1-11通道  PC1 DMA传输
uint16_t ADC_DMA_Data;
static void ADC_Gpio_Config(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;
	//ADC采用模拟输入
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOC, &GPIO_InitStruct);
}

//配置模式时注意使能的顺序，可能会影响结果
static void ADC_Mode_Config(void)
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

	//配置DMA
	//ADC1 对应可以请求 DMA1.CHANEEL1
	
	//打开DMA时钟
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	
	//DMA初始化
	DMA_InitTypeDef	DMA_InitStructure;
	//从ADC的DR寄存器中读取数据送往SRAM中
	//由于ADC的精度只有12位，所以选用 uint16_t 即可
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(ADC1->DR);
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&ADC_DMA_Data;
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
	
	//配置ADDCLK
	RCC_ADCCLKConfig(RCC_PCLK2_Div8);
	
	//使能ADC
	ADC_Cmd(ADC1,ENABLE);	
	
	//ADC发送DMA请求
	ADC_DMACmd(ADC1,ENABLE);
	
	//规则通道配置
	ADC_RegularChannelConfig(ADC1,ADC_Channel_11,1,ADC_SampleTime_41Cycles5);
	
	//触发之前校准
	ADC_StartCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1)==RESET);
	
	//软件触发
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);
}



void ADC_DMA_Config(void)
{
	ADC_Gpio_Config();
	ADC_Mode_Config();
}


