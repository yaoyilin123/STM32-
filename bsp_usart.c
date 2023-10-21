#include "bsp_usart.h"

static void NVIC_Config()
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitStructure.NVIC_IRQChannel = DEBUG_USART_IRQ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}


void USART_Config(void)
{
	//使能USART1使用到的CLK TX（PA9） RX（PA10） 
	GPIO_InitTypeDef GPIO_InitStruct;
	//首先使能USART和GPIO的时钟
	RCC_APB2PeriphClockCmd(DEBUG_USART_CLK,ENABLE);   
	RCC_APB2PeriphClockCmd(DEBUG_USART_GPIO_CLK,ENABLE);
	GPIO_InitStruct.GPIO_Pin = (DEBUG_USART_TX_GPIO_PIN );
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(DEBUG_USART_TX_GPIO_PORT , &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = (DEBUG_USART_RX_GPIO_PIN );
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING; //RX接收时，是接收到什么就是什么 所以应该选择浮空 不能上拉或者下拉
	GPIO_Init(DEBUG_USART_RX_GPIO_PORT, &GPIO_InitStruct);
	
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
	//配置串口模式-> 收发一起 
	USART_InitStructure.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
	//配置硬件流控制
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Init(DEBUG_USARTx ,&USART_InitStructure );
	
	// 串口中断优先级配置
	NVIC_Config();
	//串口中断使能
	USART_ITConfig(DEBUG_USARTx,USART_IT_RXNE, ENABLE); //RXNE为读取寄存器非空，即传入了数据可以读取，就进入中断
	//使能串口
	USART_Cmd(DEBUG_USARTx, ENABLE);
	
}

void Usart_SendBit(USART_TypeDef* PUSARTx, uint16_t Data)
{
	//传入的串口与数据
	USART_SendData(PUSARTx,Data);
	//等待发送数据寄存器为空
	while(USART_GetITStatus(PUSARTx,USART_IT_TXE) == RESET);
}


