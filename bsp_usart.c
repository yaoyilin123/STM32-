#include "bsp_usart.h"
#include <stdio.h>
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
	//USART_ITConfig(DEBUG_USARTx,USART_IT_RXNE, ENABLE); //RXNE为读取寄存器非空，即传入了数据可以读取，就进入中断
	//使能串口
	USART_Cmd(DEBUG_USARTx, ENABLE);
	
}

void Usart_SendBit(USART_TypeDef* pUSARTx, uint8_t Data)
{
	//传入的串口与数据
	USART_SendData(pUSARTx,Data);
	//等待发送数据寄存器为空
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET); //TXE->发送数据寄存器空 
}

void Usart_SendHalfWord(USART_TypeDef* pUSARTx, uint16_t Data)
{
	uint8_t temp_H,temp_L;
	temp_H = (Data&0XFF00)>>8;
	temp_L = (Data&0xFF);
	//传入的串口与数据
	USART_SendData(pUSARTx,temp_H);
	//等待发送数据寄存器为空
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
		//传入的串口与数据
	USART_SendData(pUSARTx,temp_L);
	//等待发送数据寄存器为空
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
}



void Usart_SendArr(USART_TypeDef* pUSARTx, uint8_t *arr,uint8_t num)
{
	uint8_t i;
	for(i=0;i<num;i++)
	{
			Usart_SendBit(pUSARTx, arr[i]);//循环 每个字节的发送
	}
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TC) == RESET);  //TC->发送完成
}



void Usart_SendStr(USART_TypeDef* pUSARTx,char *str)
{
	uint8_t i= 0 ;
	do
	{	
			Usart_SendBit(pUSARTx,*(str+i));
			i++;
	}while(*(str+i) !='\0');
	 while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET); //等待发送完成
}


///重定向c库函数printf到串口，重定向后可使用printf函数
int fputc(int ch, FILE *f)
{
		/* 发送一个字节数据到串口 */
		USART_SendData(DEBUG_USARTx, (uint8_t) ch);
		
		/* 等待发送完毕 */
		while (USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_TXE) == RESET);		
	
		return (ch);
}

///重定向c库函数scanf到串口，重写向后可使用scanf、getchar等函数
int fgetc(FILE *f)
{
		/* 等待串口输入数据 */
		while (USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(DEBUG_USARTx);
}

void showMessage(void)
{
	printf(" 请输入数字（1-3）来点亮RGB灯\n");
	printf("1   ——   红色\n");
	printf("2   ——   绿色\n");
	printf("3   ——   蓝色\n");
}









