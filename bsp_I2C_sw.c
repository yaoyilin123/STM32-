#include "bsp_I2C_sw.h"
#include <stdio.h>

//利用软件模拟I2C  SCL - PB6   SDA - PB7
void I2C_SW_GPIO_Config(void)
{
	//配置对应GPIO
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7 ;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStruct);
}


//用于产生开始信号
void I2C_Start(void)
{
	EEPROM_I2C_SCL_1();
	EEPROM_I2C_SDA_1();
	i2c_Delay();
	EEPROM_I2C_SDA_0();
	i2c_Delay();
	EEPROM_I2C_SCL_0();
	i2c_Delay();
}


//用于产生结束信号
void I2C_Stop(void)
{
	EEPROM_I2C_SDA_0();
	EEPROM_I2C_SCL_1();
	i2c_Delay();
	EEPROM_I2C_SDA_1();
}

void I2C_SendByte(uint8_t ByteToSend)
{
	uint8_t i;
	//发送先高位字节
	for(i=0;i<8;i++)
	{
		if(ByteToSend &0x80) //判断高位是否为0 ,如果是高位是1 ，SDA置1，然后再置SCL为1接收有效数据
		{
		EEPROM_I2C_SDA_1();
		}
		else
		{
		EEPROM_I2C_SDA_0();
		}
		i2c_Delay();
		EEPROM_I2C_SCL_1();
		i2c_Delay();
		EEPROM_I2C_SCL_0(); //读取结束置0
		if(7 == i)//最后一次时，释放总线,将SDA置1
		{
			EEPROM_I2C_SDA_1();
		}
		ByteToSend <<= 1; //每次读取完高位以后左移，再次读取最高位
		i2c_Delay();
	}
}

uint8_t ReadByte(void)
{
	uint8_t i,value=0;
	for(i=0;i<8;i++)
	{
		value<<=1;
		EEPROM_I2C_SCL_1(); //开始接收数据
		i2c_Delay();
		if(EEPROM_I2C_SDA_READ())
		{
			value++;
		}
		else{}
		EEPROM_I2C_SCL_0(); //开始接收数据
		i2c_Delay();
	}
	return value;
}


void I2C_Ask(void)
{
	EEPROM_I2C_SDA_0();
	i2c_Delay();
	EEPROM_I2C_SCL_1();
	i2c_Delay();
	EEPROM_I2C_SCL_0();
	i2c_Delay();
	EEPROM_I2C_SDA_1();
}

void I2C_NAsk(void)
{
	EEPROM_I2C_SDA_1();
	i2c_Delay();
	EEPROM_I2C_SCL_1();
	i2c_Delay();
	EEPROM_I2C_SCL_0();
}


uint8_t I2C_WaitAsk(void)
{
	uint8_t re;
	
	EEPROM_I2C_SDA_1();	/* CPU释放SDA总线 */
	i2c_Delay();
	EEPROM_I2C_SCL_1();	/* CPU驱动SCL = 1, 此时器件会返回ACK应答 */
	i2c_Delay();
	
	if (EEPROM_I2C_SDA_READ())	/* CPU读取SDA口线状态 */
		//【注意】SDA为0时为应答
	{
		re = 1;
	}
	else
	{
		re = 0;
	}
	EEPROM_I2C_SCL_0();
	i2c_Delay();
	return re;
	
}

/*
*********************************************************************************************************
*	函 数 名: i2c_Delay
*	功能说明: I2C总线位延迟，最快400KHz
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void i2c_Delay(void)
{
	uint8_t i;

	/*　
	 	下面的时间是通过逻辑分析仪测试得到的。
    工作条件：CPU主频72MHz ，MDK编译环境，1级优化
  
		循环次数为10时，SCL频率 = 205KHz 
		循环次数为7时，SCL频率 = 347KHz， SCL高电平时间1.5us，SCL低电平时间2.87us 
	 	循环次数为5时，SCL频率 = 421KHz， SCL高电平时间1.25us，SCL低电平时间2.375us 
	*/
	for (i = 0; i < 10; i++);
}


uint8_t I2C_EEPROM_SW_READBytes(uint8_t AddrToRead,uint8_t *pReadBuffer,uint8_t NumToRead)
{
	uint8_t i ;
	//1.起始信号
	I2C_Start();
	//2.发送从设备地址 EEPROM ，读/写模式
	I2C_SendByte(EEPROM_ADDR | EEPROM_I2C_WR);
	//3.等待应答
	if(0 != I2C_WaitAsk())
	{
		goto CmD_fail;
	}
	//4.发送EEPROM内部存储单元地址
	I2C_SendByte(AddrToRead);
	//5.等待应答
	if(0 != I2C_WaitAsk())
	{
		goto CmD_fail;
	}
	//6.第二次起始信号
	I2C_Start();
	//7.发送从设备EEPROM地址，并标注读/写模式
	I2C_SendByte(EEPROM_ADDR | EEPROM_I2C_RD);
	//8.等待应答
	if(0 != I2C_WaitAsk())
	{
		goto CmD_fail;
	}
		//9.循环读取数据
		for(i=0;i<NumToRead;i++)
	{
		//存储到目标位置
		pReadBuffer[i]= ReadByte();
		//到最后一个字节时回复NASK
		if(i !=NumToRead-1)
		{
			I2C_Ask();
		}
		else
		{
			I2C_NAsk();
		}
	}
	//10.发送结束信号
	I2C_Stop();
	//11.正常结束
	return 1;
	
CmD_fail:
	I2C_Stop();
	printf("读取时Ask无应答");
	return 0;
}


uint8_t I2C_EEPROM_SW_WRITEBytes(uint8_t AddrToWrite,uint8_t *BytesToWrite,uint8_t NumToWrite)
{
	uint16_t i,m;
	uint8_t Addr;
	
	Addr = AddrToWrite;
	
		for (i = 0; i < NumToWrite; i++)
	{
		/* 当发送第1个字节或是页面首地址时，需要重新发起启动信号和地址 */
		if ((i == 0) || (Addr & (8 - 1)) == 0)
		{
			/*　第０步：发停止信号，启动内部写操作　*/
			I2C_Stop();
			
			/* 通过检查器件应答的方式，判断内部写操作是否完成, 一般小于 10ms 			
				CLK频率为200KHz时，查询次数为30次左右
			*/
			for (m = 0; m < 1000; m++)
			{				
				/* 第1步：发起I2C总线启动信号 */
				I2C_Start();
				
				/* 第2步：发起控制字节，高7bit是地址，bit0是读写控制位，0表示写，1表示读 */
				I2C_SendByte(EEPROM_ADDR | EEPROM_I2C_WR);	/* 此处是写指令 */
				
				/* 第3步：发送一个时钟，判断器件是否正确应答 */
				if (I2C_WaitAsk() == 0)
				{
					break;
				}
			}
			if (m  == 1000)
			{
				goto cmd_fail;	/* EEPROM器件写超时 */
			}
		
			/* 第4步：发送字节地址，24C02只有256字节，因此1个字节就够了，如果是24C04以上，那么此处需要连发多个地址 */
			I2C_SendByte((uint8_t)Addr);
			
			/* 第5步：等待ACK */
			if (I2C_WaitAsk() != 0)
			{
				goto cmd_fail;	/* EEPROM器件无应答 */
			}
		}
	
		/* 第6步：开始写入数据 */
		I2C_SendByte(BytesToWrite[i]);
	
		/* 第7步：发送ACK */
		if (I2C_WaitAsk() != 0)
		{
			goto cmd_fail;	/* EEPROM器件无应答 */
		}

		Addr++;	/* 地址增1 */		
	}
	
	/* 命令执行成功，发送I2C总线停止信号 */
	I2C_Stop();
	return 1;

cmd_fail: /* 命令执行失败后，切记发送停止信号，避免影响I2C总线上其他设备 */
	/* 发送I2C总线停止信号 */
	I2C_Stop();
	return 0;
	
	
	
	
	
	
	
	
//	
//	//1.起始信号
//	I2C_Start();
//	//2.发送从设备地址 EEPROM ，读/写模式
//	I2C_SendByte(EEPROM_ADDR | EEPROM_I2C_WR);
//	//3.等待应答
//	if(0 != I2C_WaitAsk())
//	{
//		goto Cmd_fail;
//	}
//	//4.发送EEPROM内部存储单元地址
//	I2C_SendByte(AddrToWrite);
//	//5.等待应答
//	if(0 != I2C_WaitAsk())
//	{
//		goto Cmd_fail;
//	}
//	//6.循环写入数据内容
//	for(i=0;i<NumToWrite;i++)
//	{
//		I2C_SendByte(BytesToWrite[i]);
//		if(0 != I2C_WaitAsk())
//		{
//			goto Cmd_fail;
//		}
//		AddrToWrite++;
//	}
//	//7.产生结束信号
//	I2C_Stop();
//	return 1;
//	
//Cmd_fail:
//	I2C_Stop();
//	printf("写入时Ask无应答");
//	return 0;
}

static void ee_Delay(__IO uint32_t nCount)	 //简单的延时函数
{
	for(; nCount != 0; nCount--);
}



void Test_1(void)
{
	uint16_t i;
	uint8_t write_buf[8]={1,2,3,4,5,6,7,8};
  uint8_t read_buf[8]={1,1,1,1,1,1,1,1};
	uint8_t W1,R1;
	//写入测试
	
	W1 = I2C_EEPROM_SW_WRITEBytes(0,write_buf,8);
	if( W1)
	printf("\n写入结束\n");
	else
	{
	printf("\n写入失败\n");
	}
	//写入后给予适当的延时
  ee_Delay(0x0FFFFF);
	
	//读出测试
	R1 = I2C_EEPROM_SW_READBytes(0,read_buf,8);
	if(R1)
	printf("\n读取结束\n");
	else
	{
	printf("\n读取失败\n");
	}
	
	//打印读出的数据
	printf("读出的数据是：");
	for(i=0;i<8;i++)
	{
	printf("%d ",read_buf[i]);
	}
}












