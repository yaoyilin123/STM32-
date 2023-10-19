#ifndef   __BSP_ADVANCETIM_H
#define   __BSP_ADVANCETIM_H

#include "stm32f10x.h"

#define TIM1_CH_GPIO_PIN     	  GPIO_Pin_8
#define TIM1_CH_GPIO_PORT   		GPIOA

#define TIM1_CHN_GPIO_PIN				GPIO_Pin_13
#define TIM1_CHN_GPIO_PORT   		GPIOB

#define TIM1_CHBK_GPIO_PIN			GPIO_Pin_12
#define TIM1_CHBK_GPIO_PORT   	GPIOB

void AdvanceTim_Config(void);
#endif   /* __BSP_ADVANCETIM_H */
