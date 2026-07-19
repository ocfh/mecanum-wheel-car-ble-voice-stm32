#ifndef __HC_SR04_H__
#define __HC_SR04_H__
#include "stm32f4xx.h"
#include "Motor.h"
#include "LED.h"
#include "delay.h"

// --------------- 超声波模块宏定义 ---------------
#define TRIG_PIN    GPIO_Pin_11
#define TRIG_PORT   GPIOD
#define ECHO_PIN    GPIO_Pin_10
#define ECHO_PORT   GPIOD

#define HCSR04_TIM  TIM4       // 用于ECHO计时的定时器
#define HCSR04_CLK  RCC_APB1Periph_TIM4
#define HCSR04_IRQ  TIM4_IRQn  // 定时器中断

#define DISTANCE_THRESHOLD  25  // 避障阈值(cm)，小于该值触发避障
#define MAX_DISTANCE        400 // 超声波最大测量距离(cm)

//----------------外部变量------------------
//中断函数变量
extern volatile uint32_t echo_start_time;  // ECHO上升沿时间戳
extern volatile uint32_t echo_end_time;    // ECHO下降沿时间戳
extern volatile uint8_t echo_flag;         // ECHO捕获标志

//主函数变量
extern volatile uint8_t auto_avoid_flag;	// 自动避障总开关
extern volatile uint8_t avoid_action_flag;	// 避障动作状态：0=空闲  1=正在执行避障动作
//------------------ End --------------------


// --------------- 超声波函数声明 ---------------
void HCSR04_Init(void);                // 超声波模块初始化
void HCSR04_Trigger(void);             // 发送触发信号
float HCSR04_GetDistance(void);        // 获取测量距离
void HCSR04_Avoidance(float distance);           // 避障逻辑处理

#endif
