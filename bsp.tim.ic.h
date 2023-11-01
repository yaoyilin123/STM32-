#ifndef   __BSP_TIM_IC_H
#define   __BSP_TIM_IC_H

#include "stm32f10x.h"

typedef struct
{
	uint8_t 	InputCapture_StartFlag;   //起始标志位
	uint8_t 	InputCapture_FinishiFlag; //结束标志位
	uint16_t  InputCapture_CCRValue; 		//CCR计数值
	uint16_t  InputCapture_Period;			//溢出次数
}TIM_InputCapture_TypeInit;

extern TIM_InputCapture_TypeInit InputCapture_TypeStructure;

void TIM_IC_exam_Config(void);
#endif   /* __BSP_TIM_IC_H */
