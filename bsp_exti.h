#ifndef  __BSP_EXTI_H
#define  __BSP_EXTI_H

#include "stm32f10x.h"

#define KEY1_GPIO_PIN       GPIO_Pin_0
#define KEY1_GPIO_PORT 			GPIOA 
#define KEY1_GPIO_CLK		  	RCC_APB2Periph_GPIOA


void EXTI_Config(void);
#endif  /*__BSP_EXTI_H*/
