#include "Motor.h"

// ============== 原有电机GPIO初始化 ==============
void Motor_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA |
                           RCC_AHB1Periph_GPIOB |
                           RCC_AHB1Periph_GPIOC |
                           RCC_AHB1Periph_GPIOD |
                           RCC_AHB1Periph_GPIOE, ENABLE);

    GPIO_InitStructure.GPIO_Pin   = MOTOR_EN_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_Init(MOTOR_EN_PORT, &GPIO_InitStructure);
    GPIO_SetBits(MOTOR_EN_PORT, MOTOR_EN_PIN);

    GPIO_InitStructure.GPIO_Pin = MR1_IN1_PIN | MR1_IN2_PIN;
    GPIO_Init(MR1_IN1_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = MR2_IN1_PIN | MR2_IN2_PIN;
    GPIO_Init(MR2_IN1_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = ML1_IN1_PIN | ML1_IN2_PIN;
    GPIO_Init(ML1_IN1_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = ML2_IN1_PIN | ML2_IN2_PIN;
    GPIO_Init(ML2_IN1_PORT, &GPIO_InitStructure);
}

// ============== 原有电机PWM初始化(TIM3) ==============
void Motor_PWM_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB, ENABLE);

    GPIO_InitStructure.GPIO_Pin   = MR1_PWM_PIN | MR2_PWM_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_Init(MR1_PWM_PORT, &GPIO_InitStructure);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_TIM3);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_TIM3);

    GPIO_InitStructure.GPIO_Pin   = ML1_PWM_PIN | ML2_PWM_PIN;
    GPIO_Init(ML1_PWM_PORT, &GPIO_InitStructure);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource0, GPIO_AF_TIM3);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource1, GPIO_AF_TIM3);

    TIM_TimeBaseStructure.TIM_Period = 999;
    TIM_TimeBaseStructure.TIM_Prescaler = 83;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

    TIM_OCInitStructure.TIM_OCMode      = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse       = 0;
    TIM_OCInitStructure.TIM_OCPolarity   = TIM_OCPolarity_High;

    TIM_OC1Init(TIM3, &TIM_OCInitStructure);
    TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
    TIM_OC2Init(TIM3, &TIM_OCInitStructure);
    TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
    TIM_OC3Init(TIM3, &TIM_OCInitStructure);
    TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
    TIM_OC4Init(TIM3, &TIM_OCInitStructure);
    TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);

    TIM_Cmd(TIM3, ENABLE);
}

// ============== 原有电机调速函数 ==============
void Motor_SetSpeed(uint8_t motor, int16_t speed)
{
    uint16_t pwm_val;
    if (speed > 1000) speed = 1000;
    if (speed < -1000) speed = -1000;

    if (speed == 0)
    {
        switch(motor)
        {
            case MOTOR_RIGHT_FRONT:
                GPIO_ResetBits(MR1_IN1_PORT, MR1_IN1_PIN);
                GPIO_ResetBits(MR1_IN2_PORT, MR1_IN2_PIN);
                TIM_SetCompare1(TIM3, 0);
                break;
            case MOTOR_RIGHT_REAR:
                GPIO_ResetBits(MR2_IN1_PORT, MR2_IN1_PIN);
                GPIO_ResetBits(MR2_IN2_PORT, MR2_IN2_PIN);
                TIM_SetCompare2(TIM3, 0);
                break;
            case MOTOR_LEFT_FRONT:
                GPIO_ResetBits(ML1_IN1_PORT, ML1_IN1_PIN);
                GPIO_ResetBits(ML1_IN2_PORT, ML1_IN2_PIN);
                TIM_SetCompare3(TIM3, 0);
                break;
            case MOTOR_LEFT_REAR:
                GPIO_ResetBits(ML2_IN1_PORT, ML2_IN1_PIN);
                GPIO_ResetBits(ML2_IN2_PORT, ML2_IN2_PIN);
                TIM_SetCompare4(TIM3, 0);
                break;
        }
        return;
    }

    if (speed > 0)
    {
        pwm_val = speed;
        switch(motor)
        {
            case MOTOR_RIGHT_FRONT: GPIO_SetBits(MR1_IN1_PORT, MR1_IN1_PIN); GPIO_ResetBits(MR1_IN2_PORT, MR1_IN2_PIN); break;
            case MOTOR_RIGHT_REAR:  GPIO_SetBits(MR2_IN1_PORT, MR2_IN1_PIN); GPIO_ResetBits(MR2_IN2_PORT, MR2_IN2_PIN); break;
            case MOTOR_LEFT_FRONT:  GPIO_SetBits(ML1_IN1_PORT, ML1_IN1_PIN); GPIO_ResetBits(ML1_IN2_PORT, ML1_IN2_PIN); break;
            case MOTOR_LEFT_REAR:   GPIO_SetBits(ML2_IN1_PORT, ML2_IN1_PIN); GPIO_ResetBits(ML2_IN2_PORT, ML2_IN2_PIN); break;
        }
    }
    else
    {
        pwm_val = -speed;
        switch(motor)
        {
            case MOTOR_RIGHT_FRONT: GPIO_ResetBits(MR1_IN1_PORT, MR1_IN1_PIN); GPIO_SetBits(MR1_IN2_PORT, MR1_IN2_PIN); break;
            case MOTOR_RIGHT_REAR:  GPIO_ResetBits(MR2_IN1_PORT, MR2_IN1_PIN); GPIO_SetBits(MR2_IN2_PORT, MR2_IN2_PIN); break;
            case MOTOR_LEFT_FRONT:  GPIO_ResetBits(ML1_IN1_PORT, ML1_IN1_PIN); GPIO_SetBits(ML1_IN2_PORT, ML1_IN2_PIN); break;
            case MOTOR_LEFT_REAR:   GPIO_ResetBits(ML2_IN1_PORT, ML2_IN1_PIN); GPIO_SetBits(ML2_IN2_PORT, ML2_IN2_PIN); break;
        }
    }

    switch(motor)
    {
        case MOTOR_RIGHT_FRONT: TIM_SetCompare1(TIM3, pwm_val); break;
        case MOTOR_RIGHT_REAR:  TIM_SetCompare2(TIM3, pwm_val); break;
        case MOTOR_LEFT_FRONT:  TIM_SetCompare3(TIM3, pwm_val); break;
        case MOTOR_LEFT_REAR:   TIM_SetCompare4(TIM3, pwm_val); break;
    }
}

// 全车停止
void Motor_StopAll(void)
{
    Motor_SetSpeed(MOTOR_RIGHT_FRONT, 0);
    Motor_SetSpeed(MOTOR_RIGHT_REAR, 0);
    Motor_SetSpeed(MOTOR_LEFT_FRONT, 0);
    Motor_SetSpeed(MOTOR_LEFT_REAR, 0);
}

// 小车运动函数
void Car_Forward(int16_t speed)
{
	//USART1_Send_String((uint8_t*)"Car_Forward\r\n"); // 临时打印
    Motor_SetSpeed(MOTOR_RIGHT_FRONT, -speed);
    Motor_SetSpeed(MOTOR_RIGHT_REAR, -speed);
    Motor_SetSpeed(MOTOR_LEFT_FRONT, speed);
    Motor_SetSpeed(MOTOR_LEFT_REAR, speed);
}
void Car_Backward(int16_t speed)
{
	//USART1_Send_String((uint8_t*)"Car_Backward\r\n"); // 临时打印
    Motor_SetSpeed(MOTOR_RIGHT_FRONT, speed);
    Motor_SetSpeed(MOTOR_RIGHT_REAR, speed);
    Motor_SetSpeed(MOTOR_LEFT_FRONT, -speed);
    Motor_SetSpeed(MOTOR_LEFT_REAR, -speed);
}
void Car_Left(int16_t speed)
{
	//USART1_Send_String((uint8_t*)"Car_Left\r\n"); // 临时打印
    Motor_SetSpeed(MOTOR_RIGHT_FRONT, -speed);
    Motor_SetSpeed(MOTOR_RIGHT_REAR, -speed);
    Motor_SetSpeed(MOTOR_LEFT_FRONT, -speed);
    Motor_SetSpeed(MOTOR_LEFT_REAR, -speed);
}
void Car_Right(int16_t speed)
{
	//USART1_Send_String((uint8_t*)"Car_Right\r\n"); // 临时打印
    Motor_SetSpeed(MOTOR_RIGHT_FRONT, speed);
    Motor_SetSpeed(MOTOR_RIGHT_REAR, speed);
    Motor_SetSpeed(MOTOR_LEFT_FRONT, speed);
    Motor_SetSpeed(MOTOR_LEFT_REAR, speed);
}

