#ifndef __BSP_LED_H
#define __BSP_LED_H

#include "stm32f10x.h"

#define LED_R_GPIO_PIN      GPIO_Pin_5
#define LED_R_GPIO_PORT 		GPIOB 
#define LED_R_GPIO_CLK			RCC_APB2Periph_GPIOB

#define LED_G_GPIO_PIN      GPIO_Pin_0
#define LED_G_GPIO_PORT 		GPIOB 
#define LED_G_GPIO_CLK			RCC_APB2Periph_GPIOB

#define LED_B_GPIO_PIN      GPIO_Pin_1
#define LED_B_GPIO_PORT 		GPIOB 
#define LED_B_GPIO_CLK			RCC_APB2Periph_GPIOB

#define ON       1
#define OFF      0

#define LED_R(x)						if(x)\
															   GPIO_ResetBits(LED_R_GPIO_PORT,LED_R_GPIO_PIN);\
														else GPIO_SetBits(LED_R_GPIO_PORT,LED_R_GPIO_PIN);

#define LED_G(x)						if(x)\
															   GPIO_ResetBits(LED_G_GPIO_PORT,LED_G_GPIO_PIN);\
														else GPIO_SetBits(LED_G_GPIO_PORT,LED_G_GPIO_PIN);
#define LED_B(x)						if(x)\
															   GPIO_ResetBits(LED_B_GPIO_PORT,LED_B_GPIO_PIN);\
														else GPIO_SetBits(LED_B_GPIO_PORT,LED_B_GPIO_PIN);

#define LED_G_TOOGLE 				{LED_G_GPIO_PORT->ODR  ^= LED_G_GPIO_PIN;}
#define LED_B_TOOGLE 				{LED_B_GPIO_PORT->ODR  ^= LED_B_GPIO_PIN;}
#define LED_R_TOOGLE 				{LED_R_GPIO_PORT->ODR  ^= LED_R_GPIO_PIN;}

#define LED_RED 						LED_R(ON);LED_B(OFF);LED_G(OFF)	
#define LED_BLUE 						LED_R(OFF);LED_B(ON);LED_G(OFF)	
#define LED_GREEN 					LED_R(OFF);LED_B(OFF);LED_G(ON)	
#define LED_ALLOFF					LED_R(OFF);LED_B(OFF);LED_G(OFF)	
void LED_GPIO_Config(void);
#endif /* bsp_led.h */
