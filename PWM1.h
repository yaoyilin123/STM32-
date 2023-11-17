#ifndef PWM_H
#define PWM_H

#include "stm32f10x.h"

#define Dir_Positive()		GPIO_SetBits(GPIOA,GPIO_Pin_5);
#define Dir_Reverse()			GPIO_ResetBits(GPIOA,GPIO_Pin_5);

#define Beep_ON()					GPIO_SetBits(GPIOA,GPIO_Pin_8);
#define Beep_OFF()				GPIO_ResetBits(GPIOA,GPIO_Pin_8);

typedef enum
{ 
  Motor_Speed_Fast= 72,    //72/100/72 =10 KHZ
  Motor_Speed_Common=216,  //72/100/216 = 3.3 KHZ
  Motor_Speed_Slow=360		 //72/100/360 = 2 KHZ
}MotorSpeed_TypeDef;

typedef enum
{ 
  Accel_Fast,
  Accel_Common, 
  Accel_Slow
}Accel_TypeDef;

typedef enum
{ 
  Dccel_Fast ,
  Dccel_Common, 
  Dccel_Slow
}Dccel_TypeDef;

void GeneralTim_Config(void);
void Beep_Config(void);
void MSD_Move(int step,Accel_TypeDef Accel,Dccel_TypeDef Decel,MotorSpeed_TypeDef Speed);
#endif /* PWM_H */
