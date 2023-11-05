#ifndef  __BSP_I2C_H
#define  __BSP_I2C_H

#include "stm32f10x.h"

#define EEPROM_ADD 0xa0 

void I2C_EE_Config(void);
void I2C_RandRead_Config(uint8_t add,uint8_t *data,uint8_t NumByte);
void I2C_ByetWrite_Config(uint8_t add,uint8_t data);
void detection_GenerateSta(void);
void I2C_PageWrite(uint8_t addr,uint8_t *data,uint8_t NumByte);
#endif   /*__BSP_I2C_H*/
