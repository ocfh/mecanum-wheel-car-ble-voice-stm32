#include "delay.h"

//延时微秒  注意：24bit计数器  不能798915us
void delay_us(u32 nus)
{
	SysTick->CTRL = 0; 					// 关闭定时器
	SysTick->LOAD = nus*21 - 1; // 设置重载值
	SysTick->VAL 	= 0; 					// 清除计数值
	SysTick->CTRL = 1; 					// 开启定时器  21MHZ  
	while ((SysTick->CTRL & 0x00010000)==0);// 等待时间到达
	SysTick->CTRL = 0; 					// 关闭定时器
}

//延时毫秒  注意：24bit计数器  不能798ms
void delay_ms(u32 nms)
{
	SysTick->CTRL = 0; 					// 关闭定时器
	SysTick->LOAD = nms*21000 - 1; // 设置重载值
	SysTick->VAL 	= 0; 					// 清除计数值
	SysTick->CTRL = 1; 					// 开启定时器  21MHZ  
	while ((SysTick->CTRL & 0x00010000)==0);// 等待时间到达
	SysTick->CTRL = 0; 					// 关闭定时器
}
































