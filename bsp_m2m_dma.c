#include "bsp_m2p_dma.h"
#include "bsp_usart.h"

//M To P  由SRAM传递到USART1

//在sram中创建一个变量
uint32_t aDST_Buffer1[BUFFER_SIZE1]={0x11,0x12,0x13,0x14,0x11,0x12,0x13,0x14};

void MTP_DMA_Config(void)
{
	DMA_InitTypeDef DMA_InitStruct;
	
	//打开对应时钟,
	//USART1_TX 采用DMA1 第4通道
	RCC_AHBPeriphClockCmd(MTP_DMA_CLK, ENABLE); 
	DMA_InitStruct.DMA_PeripheralBaseAddr =(uint32_t) USART_DR_ADDRESS; 
	DMA_InitStruct.DMA_MemoryBaseAddr = (uint32_t)aDST_Buffer1;
	DMA_InitStruct.DMA_DIR =DMA_DIR_PeripheralDST;  //存储器FLASH当作外设
	DMA_InitStruct.DMA_BufferSize = BUFFER_SIZE1;//传输的大小
	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable ;
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte ;  
	DMA_InitStruct.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStruct.DMA_Mode = DMA_Mode_Normal ;
	DMA_InitStruct.DMA_Priority = DMA_Priority_High;
	DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(MTP_DMA_CHANNEL,&DMA_InitStruct);
	//清楚标志位
	DMA_ClearFlag(DMA1_FLAG_TC4);
	//DMA使能
	DMA_Cmd(MTP_DMA_CHANNEL,ENABLE);
	
	//使能USART1使用到的CLK TX（PA9）
	GPIO_InitTypeDef GPIO_InitStruct;
	//首先使能USART和GPIO的时钟
	RCC_APB2PeriphClockCmd(DEBUG_USART_CLK,ENABLE);   
	RCC_APB2PeriphClockCmd(DEBUG_USART_GPIO_CLK,ENABLE);
	GPIO_InitStruct.GPIO_Pin = (DEBUG_USART_TX_GPIO_PIN );
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(DEBUG_USART_TX_GPIO_PORT , &GPIO_InitStruct);

	//USART初始化
	USART_InitTypeDef USART_InitStructure;
	//配置波特率
	USART_InitStructure.USART_BaudRate = DEBUG_USART_BAUDRATE;
	//配置针数据字长
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	//配置停止位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	//配置校验位
	USART_InitStructure.USART_Parity = USART_Parity_No;
	//配置串口模式
	USART_InitStructure.USART_Mode = USART_Mode_Tx;
	//配置硬件流控制
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Init(DEBUG_USARTx ,&USART_InitStructure );
	//使能串口
	USART_Cmd(DEBUG_USARTx, ENABLE);
	
}

