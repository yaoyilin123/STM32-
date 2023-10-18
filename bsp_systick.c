#include "bsp_systick.h"
#include "core_cm3.h"

//systick 属于内核

void SysTick_Delay_ms(uint32_t ms)
{
	uint32_t i;
	//设置重装载寄存器的值
	SysTick_Config(72000000/1000) ;   //Config内部设置好各种参数 CTRL LOAD VAL
	
	for(i=0;i<ms;i++)
	{
		while(!(SysTick->CTRL &(1<<16))); //CTRL的第十六位在计时器数到值时变为1，在读取第十六位，会置0
	}
	
	//关闭定时器 让SysTick的CTRL第0位置0
	SysTick->CTRL &=~ SysTick_CTRL_ENABLE_Msk;
	//SysTick->CTRL &=~(1<<0);
	
}


void SysTick_Delay_us(uint32_t us)
{
	uint32_t i;
	//设置重装载寄存器的值
	SysTick_Config(72000000/1000000) ;   //Config内部设置好各种参数 CTRL LOAD VAL
	
	for(i=0;i<us;i++)
	{
		while(!(SysTick->CTRL &(1<<16)));
	}
	
	//关闭定时器 让SysTick的CTRL第0位置0
	SysTick->CTRL &=~ SysTick_CTRL_ENABLE_Msk;
	//SysTick->CTRL &=~(1<<0);
	
}

