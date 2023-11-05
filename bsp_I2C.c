#include "bsp_I2C.h"
#include <stdio.h>


//使用I2C1 对应端口为PB6—SCL   PB7—SDA
static void I2C_GPIO_Config(void)
{
	//配置对应GPIO
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 ;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;
  GPIO_Init(GPIOB, &GPIO_InitStruct);
}

//配置对应I2C结构体
static void I2C_Mode_Config(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1,ENABLE);
	I2C_InitTypeDef	I2C_InitStructure;
	
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable ; //应答
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed = 400000;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_OwnAddress1 = 0x5f; 	 //这个地址只要跟I2C的地址不同即可 ，设置7位
	
	I2C_Init(I2C1,&I2C_InitStructure);
	
	//I2C使能
	I2C_Cmd(I2C1,ENABLE);
}

void I2C_EE_Config(void)
{
	I2C_GPIO_Config();
	I2C_Mode_Config();
}


//字写入 byte-write
void I2C_ByetWrite_Config(uint8_t add,uint8_t data)
{
	//发送开始位
	I2C_GenerateSTART(I2C1,ENABLE);
	//检验发送位—EV5事件-SB是否置1
	while(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT)==ERROR);

	//发送EEPROM地址，等待其应答
	I2C_Send7bitAddress(I2C1,EEPROM_ADD,I2C_Direction_Transmitter);  //****************EEPROM地址*******************
	while(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)==ERROR);
	
	//发送数据 【注意】发送数据后，TXE寄存器自动清空，无需手动清楚
	I2C_SendData(I2C1,add); //此时传入的时EEPROM内部的存储单元地址
	while(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTING)==ERROR);
	
	//发送需要传递的数据
	I2C_SendData(I2C1,data);
	while(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED)==ERROR);
	
	//发送结束位
	I2C_GenerateSTOP(I2C1,ENABLE);
}

//页写入
//F103VET6的EEPROM为256K的，最多一次性写入8个 需要注意 NumByte 不要大于8
void I2C_PageWrite(uint8_t addr,uint8_t *data,uint8_t NumByte)  
{
	//发送开始位
	I2C_GenerateSTART(I2C1,ENABLE);
	//检验发送位—EV5事件-SB是否置1
	while(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT)==ERROR);

	//发送EEPROM地址，等待其应答
	I2C_Send7bitAddress(I2C1,EEPROM_ADD,I2C_Direction_Transmitter);  //****************EEPROM地址*******************
	while(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)==ERROR);
	
	//发送数据 【注意】发送数据后，TXE寄存器自动清空，无需手动清楚
	I2C_SendData(I2C1,addr); //此时传入的时EEPROM内部的存储单元地址
	while(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTING)==ERROR);
	
	while(NumByte)
	{
		//发送需要传递的数据
		I2C_SendData(I2C1,*data);

		if(NumByte ==1)
		{
			while(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED)==ERROR);
		}
		else
		{
			while(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTING)==ERROR);
		}

		NumByte--;
		data++;
	}
	//发送结束位
	I2C_GenerateSTOP(I2C1,ENABLE);

}





//读取 ————最多读取256个
void I2C_RandRead_Config(uint8_t add,uint8_t *data,uint8_t NumByte)
{

		//发送开始位
	I2C_GenerateSTART(I2C1,ENABLE);
	//检验发送位—EV5事件—SB是否置1
	while(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT)==ERROR);
	
	//发送EEPROM地址，等待其应答
	I2C_Send7bitAddress(I2C1,EEPROM_ADD,I2C_Direction_Transmitter); 
	while(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)==ERROR);

	//发送内存单元地址
	I2C_SendData(I2C1,add);
	while(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED)==ERROR);
	
	//第二次起始信号
	I2C_GenerateSTART(I2C1,ENABLE);
	while(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT)==ERROR);
	
	I2C_Send7bitAddress(I2C1,EEPROM_ADD,I2C_Direction_Receiver); 
	while(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)==ERROR);
	
	//检测有效数据传过来
	while(NumByte)
	{
		if(NumByte==1)
		{
			I2C_AcknowledgeConfig(I2C1,DISABLE);
		}
		while(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_RECEIVED)==ERROR);
		*data = I2C_ReceiveData(I2C1); //将收到的信息存储起来
		data++;
		NumByte--;
	}

	
	I2C_GenerateSTOP(I2C1,ENABLE);
	
	//使其默认为使能，方便下次接收
	I2C_AcknowledgeConfig(I2C1,ENABLE);
}




//等待EEPROM写入时序的完成
void detection_GenerateSta(void) 
{
	do
	{
		//发送开始位
	I2C_GenerateSTART(I2C1,ENABLE);
	//检验发送位—EV5事件—SB是否置1
	while(I2C_GetFlagStatus(I2C1,I2C_FLAG_SB)==RESET);
	
	//发送EEPROM地址，等待其应答
	I2C_Send7bitAddress(I2C1,EEPROM_ADD,I2C_Direction_Transmitter); 
	}
	while(I2C_GetFlagStatus(I2C1,I2C_FLAG_ADDR)==RESET);
	//EEPROM内部时序传输完成,生成结束位
	I2C_GenerateSTOP(I2C1,ENABLE);


}
