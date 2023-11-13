#ifndef __BSP_ADC_TEP_H
#define __BSP_ADC_TEP_H

#include "stm32f10x.h"

//V25 平均值为1.43V   
//ADC12位精度 0xFFF对应3.3V  换算1.43V = 0x6EE
#define V25  			0x6EE

//斜率 每摄氏度4.3mV 对应每摄氏度0x05
#define AVG_SLOPE 0x05 

void ADCx_Mode_Config(void);

#endif /* bsp_adc_tep.h */
