#ifndef  __BSP_I2C_SW_H
#define  __BSP_I2C_SW_H

#include "stm32f10x.h"

#define EEPROM_ADDR 0xa0 

#define EEPROM_I2C_WR	0		/* 写控制bit */
#define EEPROM_I2C_RD	1		/* 读控制bit */



#define EEPROM_I2C_SCL_1() 		GPIO_SetBits(GPIOB,GPIO_Pin_6)
#define EEPROM_I2C_SCL_0()  	GPIO_ResetBits(GPIOB,GPIO_Pin_6)

#define EEPROM_I2C_SDA_1()  	GPIO_SetBits(GPIOB,GPIO_Pin_7)
#define EEPROM_I2C_SDA_0()  	GPIO_ResetBits(GPIOB,GPIO_Pin_7)

#define EEPROM_I2C_SDA_READ()  ((GPIOB->IDR & GPIO_Pin_7) != 0)	/* 读SDA口线状态 */


void i2c_Delay(void);
void I2C_SW_GPIO_Config(void);
void I2C_Start(void);
void I2C_Stop(void);
void I2C_SendByte(uint8_t ByteToSend);
uint8_t ReadByte(void);
void I2C_Ask(void);
void I2C_NAsk(void);
uint8_t I2C_WaitAsk(void);
void Test_1(void);
uint8_t ee_WriteBytes(uint8_t *_pWriteBuf, uint16_t _usAddress, uint16_t _usSize);
#endif   /*__BSP_I2C_SW_H*/
