#ifndef  __BSP_M2M_DMA_H
#define  __BSP_M2M_DMA_H

#include "stm32f10x.h"


#define MTM_DMA_CLK 				RCC_AHBPeriph_DMA1
#define BUFFER_SIZE     		32

#define MTM_DMA_CHANNEL  	  DMA1_Channel3
	
	
void DMA_MTM_Config(void);
uint8_t Buffercmp(const uint32_t* pBuffer, 
                  uint32_t* pBuffer1, uint16_t BufferLength);
#endif   /* __BSP_M2M_DMA_H */
