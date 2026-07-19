#ifndef __MOTOR_H__
#define __MOTOR_H__
#include  "stm32f4xx.h"
#include  "delay.h"

#define MR1_PWM_PIN    GPIO_Pin_6
#define MR1_PWM_PORT   GPIOA
#define MR1_IN1_PIN    GPIO_Pin_4
#define MR1_IN1_PORT   GPIOE
#define MR1_IN2_PIN    GPIO_Pin_5
#define MR1_IN2_PORT   GPIOE

#define MR2_PWM_PIN    GPIO_Pin_7
#define MR2_PWM_PORT   GPIOA
#define MR2_IN1_PIN    GPIO_Pin_3
#define MR2_IN1_PORT   GPIOE
#define MR2_IN2_PIN    GPIO_Pin_2
#define MR2_IN2_PORT   GPIOE

#define ML1_PWM_PIN    GPIO_Pin_0
#define ML1_PWM_PORT   GPIOB
#define ML1_IN1_PIN    GPIO_Pin_12
#define ML1_IN1_PORT   GPIOD
#define ML1_IN2_PIN    GPIO_Pin_13
#define ML1_IN2_PORT   GPIOD

#define ML2_PWM_PIN    GPIO_Pin_1
#define ML2_PWM_PORT   GPIOB
#define ML2_IN1_PIN    GPIO_Pin_14
#define ML2_IN1_PORT   GPIOD
#define ML2_IN2_PIN    GPIO_Pin_15
#define ML2_IN2_PORT   GPIOD

#define MOTOR_EN_PIN   GPIO_Pin_4
#define MOTOR_EN_PORT  GPIOC

typedef enum{
	MOTOR_RIGHT_FRONT = 0,
	MOTOR_RIGHT_REAR,
	MOTOR_LEFT_FRONT,
	MOTOR_LEFT_REAR ,
}Motor_sign;

// --------------- º¯ÊýÉùÃ÷ ---------------
void Motor_GPIO_Init(void);
void Motor_PWM_Init(void);
void Motor_SetSpeed(uint8_t motor, int16_t speed);
void Motor_StopAll(void);
void Car_Forward(int16_t speed);
void Car_Backward(int16_t speed);
void Car_Left(int16_t speed);
void Car_Right(int16_t speed);

#endif

