#ifndef __BSP_SPI_FLASH_H
#define __BSP_SPI_FLASH_H

#include "stm32f10x.h"

#define SPI_SCK_GPIO_PIN      GPIO_Pin_5
#define SPI_SCK_GPIO_PORT 		GPIOA 
#define SPI_SCK_GPIO_CLK			RCC_APB2Periph_GPIOA

#define SPI_MISO_GPIO_PIN      GPIO_Pin_6
#define SPI_MISO_GPIO_PORT 		GPIOA 
#define SPI_MISO_GPIO_CLK			RCC_APB2Periph_GPIOA

#define SPI_MOSI_GPIO_PIN      GPIO_Pin_7
#define SPI_MOSI_GPIO_PORT 		GPIOA 
#define SPI_MOSI_GPIO_CLK			RCC_APB2Periph_GPIOA

#define SPI_CS_GPIO_PIN      GPIO_Pin_0
#define SPI_CS_GPIO_PORT 		 GPIOC 
#define SPI_CS_GPIO_CLK			 RCC_APB2Periph_GPIOC

#define SPI_CS_HIGH						GPIO_SetBits(SPI_CS_GPIO_PORT,SPI_CS_GPIO_PIN)
#define SPI_CS_LOW 						GPIO_ResetBits(SPI_CS_GPIO_PORT,SPI_CS_GPIO_PIN)

#define FLASH_SPIx						SPI1

#define Dummy										0x00
#define JEDEC_ID              	0x9f
#define Sector_Erase						0x20
#define Write_Enable						0x06
#define Read_Status_Register		0x05
#define Page_Program						0x02
#define Read_Data								0x03

uint32_t SPI_ReadID(void);
uint8_t SPI_SendByte(uint8_t ByteToSend);
void SPI_FLASH_Config(void);
void SPI_Sector_Erase(uint32_t AddToErase);
void SPI_PageProgram_Write(uint32_t AddrToWrite,uint8_t *Buffer,uint16_t NumToWrite);
void SPI_ReadData(uint32_t AddrToRead,uint8_t * ReadBuffer,uint8_t NumToRead);
#endif /* __BSP_SPI_FLASH_H.h */
