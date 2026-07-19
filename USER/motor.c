#include "stm32f4xx.h"
//MR1_PWM PA6
//MR1_IN1 PB4
//MR1_IN2 PB5
void Motor_Init(){
	//打开时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOE,ENABLE);
	//配置GPIO参数
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_6;//引脚编号
	GPIO_InitStruct.GPIO_Speed=GPIO_High_Speed;//引脚速度
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_OUT;//输出模式
	//GPIO_InitStruct.GPIO_Pupd=GPIO_Pupd_;//输入模式
	GPIO_InitStruct.GPIO_OType=GPIO_OType_PP;//推挽输出
	//初始化GPIO
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	//配置GPIOE参数
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_4|GPIO_Pin_5;//引脚编号
	
	GPIO_Init(GPIOE,&GPIO_InitStruct);
}