#ifndef __STM32F10X_H
#define __STM32F10X_H

//Peripherals 外设 

#define  Perip_Base       ((unsigned int)0x40000000)
#define  APB1Perip_Base    Perip_Base 
#define  APB2Perip_Base   (Perip_Base + 0x10000)
#define  AHBPerip_Base    (Perip_Base + 0x20000)

#define RCC_BASE          (Perip_Base + 0x21000)
#define GPIOB_BASE				(APB2Perip_Base+ 0x0C00)

#define RCC_APB2ENR			 	*(unsigned int*)(RCC_BASE + 0x18)

/*#define GPIOB_CRL 				 *(unsigned int*) GPIOB_BASE
#define GPIOB_CRH 				 *(unsigned int*)(GPIOB_BASE + 0x04)
#define GPIOB_IDR 				 *(unsigned int*)(GPIOB_BASE + 0x08)
#define GPIOB_ODR 				 *(unsigned int*)(GPIOB_BASE + 0x0C)
#define GPIOB_BSRR 				 *(unsigned int*)(GPIOB_BASE + 0x10)
#define GPIOB_BRR 				 *(unsigned int*)(GPIOB_BASE + 0x14)
#define GPIOB_LCKR 				 *(unsigned int*)(GPIOB_BASE + 0x18)*/

typedef unsigned int    uint32_t;
typedef unsigned short  uint16_t;

//GPIO 结构体对应其描述
typedef struct
{
	uint32_t CRL;
	uint32_t CRH;
	uint32_t IDR;
	uint32_t ODR;
	uint32_t BSRR;
	uint32_t BRR;
	uint32_t LCKR;
}GPIO_TypeDef;
 
//RCC时钟 结构体对应其描述
typedef struct
{
	uint32_t CR;
	uint32_t CFGR;
	uint32_t CIR;
	uint32_t APB2RSTR;
	uint32_t APB1RSRT;
	uint32_t AHBENR;
	uint32_t APB2ENR;
	uint32_t APB1ENR;
	uint32_t BDCR;
	uint32_t CSR;
}RCC_TypeDef;

#define GPIOB   ((GPIO_TypeDef*)GPIOB_BASE)
#define RCC ((RCC_TypeDef *) RCC_BASE)


#endif /* __STM32F10X_H */
















 
