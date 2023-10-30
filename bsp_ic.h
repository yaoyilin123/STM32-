#ifndef __BSP_IC_H
#define __BSP_IC_H

#include "stm32f10x.h"


void OC_Config(void);
void IC_Config(void);
void Usart1_Config(void);
uint16_t IC_Get_Freq(void);
uint16_t IC_Get_Duty(void);
#endif /* bsp_IC.h */
