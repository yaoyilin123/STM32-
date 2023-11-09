#include "bsp_spi_flash.h"

static void SPI_FLASH_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_APB2PeriphClockCmd(SPI_SCK_GPIO_CLK,ENABLE);
	GPIO_InitStruct.GPIO_Pin = SPI_SCK_GPIO_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(SPI_SCK_GPIO_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin = SPI_MOSI_GPIO_PIN;
  GPIO_Init(SPI_MOSI_GPIO_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = SPI_MISO_GPIO_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(SPI_MISO_GPIO_PORT, &GPIO_InitStruct);

	RCC_APB2PeriphClockCmd(SPI_CS_GPIO_CLK,ENABLE);
	GPIO_InitStruct.GPIO_Pin = SPI_CS_GPIO_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(SPI_CS_GPIO_PORT, &GPIO_InitStruct);
}

static void SPI_FLASH_MODE_Config(void)
{
	//打开SPI时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);
	//初始化SPI
	SPI_InitTypeDef SPI_InitStructure;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
	//选用模式3 
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge ;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStructure.SPI_CRCPolynomial = 0;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	//NSS片选采用软件控制
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft; 
	
	SPI_Init(FLASH_SPIx,&SPI_InitStructure);

	SPI_Cmd(FLASH_SPIx,ENABLE);
	SPI_CS_HIGH;
}

void SPI_FLASH_Config(void)
{
	SPI_FLASH_GPIO_Config();
	SPI_FLASH_MODE_Config();
}

uint8_t SPI_SendByte(uint8_t ByteToSend)
{
//	//片选信号
//	SPI_CS_LOW;
	//检测TXE，然后发送数据
	while(SPI_I2S_GetFlagStatus(FLASH_SPIx,SPI_I2S_FLAG_TXE) == RESET);
	//发送数据
	SPI_I2S_SendData(FLASH_SPIx,ByteToSend);
	//检测数据是否接收到
	while(SPI_I2S_GetFlagStatus(FLASH_SPIx,SPI_I2S_FLAG_RXNE) == RESET);
	//传输结束，可以从接收数据寄存器中读取
//	//关闭片选信号
//	SPI_CS_HIGH;
	return 	SPI_I2S_ReceiveData(FLASH_SPIx);
}


uint32_t SPI_ReadID(void)
{
	uint32_t ID = 0;
	//CS片选信号
	SPI_CS_LOW; 
	//发送JEDEC ID
	SPI_SendByte(JEDEC_ID);
	//内部识别到指令便会返回24位
	ID = SPI_SendByte(Dummy);     //0xaa
	ID<<=8;												//0xaa00
	ID |= SPI_SendByte(Dummy);	  //0xaabb
	ID<<=8;												//0xaabb00
	ID |= SPI_SendByte(Dummy);		//0xaabbcc
	//关闭片选信号
	SPI_CS_HIGH;
	return ID;
}

//在擦除和写入的时候需要使用写使能
static void SPI_WriteEnable(void)
{
	SPI_CS_LOW; 
	SPI_SendByte(Write_Enable);
	SPI_CS_HIGH;
}

//在擦除和写入的时候需要等待，查看BUSY位是否置0
static void SPI_WaiteForEnd(void)
{
	uint8_t FlashFlag = 0;
	SPI_CS_LOW; 
	SPI_SendByte(Read_Status_Register);
	//读取BUSY位
	do
	{
		FlashFlag = SPI_SendByte(Dummy);
	}
	while((FlashFlag & 0x01) == 1);
	
	SPI_CS_HIGH;
 
}

void SPI_Sector_Erase(uint32_t AddToErase)
{	
	//开启写使能
	SPI_WriteEnable();
	uint32_t temp;
	SPI_CS_LOW; 
	//发送扇区擦除命令
	SPI_SendByte(Sector_Erase);
	//发送需要擦除的地址
	temp = AddToErase & 0xff0000;
	temp>>=16;
	SPI_SendByte(temp);
	temp = AddToErase & 0xff00;
	temp >>=8;
	SPI_SendByte(temp);
	temp = AddToErase & 0xff;
	SPI_SendByte(temp);
	SPI_CS_HIGH;              //CS线要在发送结束后就置高
	//等待其擦除结束
	SPI_WaiteForEnd();

}
 

void SPI_PageProgram_Write(uint32_t AddrToWrite,uint8_t *Buffer,uint16_t NumToWrite)
{	
	uint32_t temp;
	//开启写使能
	SPI_WriteEnable();
	SPI_CS_LOW; 
	SPI_SendByte(Page_Program);
	//发送写入的地址     								0xaabbcc
	temp = (AddrToWrite&0xff0000)>>16 ;    //0x000000aa
	SPI_SendByte(temp);
	temp = (AddrToWrite&0xff00)>>8 ;     //0x0000aabb
	SPI_SendByte(temp);
	temp = AddrToWrite & 0xff ;        //0x00aabbcc
	SPI_SendByte(temp);
	
	while(NumToWrite--)
	{
		SPI_SendByte(*Buffer);
		Buffer++;	
	}
	SPI_CS_HIGH;
	SPI_WaiteForEnd();

}

void SPI_ReadData(uint32_t AddrToRead,uint8_t * ReadBuffer,uint16_t NumToRead) 
{
	uint32_t temp;
	SPI_CS_LOW;
	SPI_SendByte(Read_Data);
	//发送写入的地址     								0xaabbcc
	temp = (AddrToRead&0xff0000)>>16 ;    //0x000000aa
	SPI_SendByte(temp);
	temp = (AddrToRead&0xff00)>>8 ;     //0x0000aabb
	SPI_SendByte(temp);
	temp = AddrToRead & 0xff ;        //0x00aabbcc
	SPI_SendByte(temp);
	
	while(NumToRead--)
	{
		*ReadBuffer = SPI_SendByte(Dummy);
		ReadBuffer++;
	}
	SPI_CS_HIGH;
}

