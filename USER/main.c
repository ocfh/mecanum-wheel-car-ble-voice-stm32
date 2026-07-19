#include "stm32f4xx.h"
#include "delay.h"
#include  "BLE.h"
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

#define LED_PWR_PIN    GPIO_Pin_13
#define LED_PWR_PORT   GPIOB

void Motor_GPIO_Init(void);
void Motor_PWM_Init(void);
void Motor_SetSpeed(uint8_t motor, int16_t speed);
void Motor_StopAll(void);
void Car_Forward(int16_t speed);
void Car_Backward(int16_t speed);
void Car_Left(int16_t speed);
void Car_Right(int16_t speed);
void LED_Init(void);
int speed;
#define MOTOR_RIGHT_FRONT  0
#define MOTOR_RIGHT_REAR   1
#define MOTOR_LEFT_FRONT   2
#define MOTOR_LEFT_REAR    3

void LED_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

    GPIO_InitStructure.GPIO_Pin   = LED_PWR_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
    GPIO_Init(LED_PWR_PORT, &GPIO_InitStructure);

    GPIO_ResetBits(LED_PWR_PORT, LED_PWR_PIN);
}
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
            case MOTOR_RIGHT_FRONT:
                GPIO_SetBits(MR1_IN1_PORT, MR1_IN1_PIN);
                GPIO_ResetBits(MR1_IN2_PORT, MR1_IN2_PIN);
                break;
            case MOTOR_RIGHT_REAR:
                GPIO_SetBits(MR2_IN1_PORT, MR2_IN1_PIN);
                GPIO_ResetBits(MR2_IN2_PORT, MR2_IN2_PIN);
                break;
            case MOTOR_LEFT_FRONT:
                GPIO_SetBits(ML1_IN1_PORT, ML1_IN1_PIN);
                GPIO_ResetBits(ML1_IN2_PORT, ML1_IN2_PIN);
                break;
            case MOTOR_LEFT_REAR:
                GPIO_SetBits(ML2_IN1_PORT, ML2_IN1_PIN);
                GPIO_ResetBits(ML2_IN2_PORT, ML2_IN2_PIN);
                break;
        }
    }
    else
    {
        pwm_val = -speed;
        switch(motor)
        {
            case MOTOR_RIGHT_FRONT:
                GPIO_ResetBits(MR1_IN1_PORT, MR1_IN1_PIN);
                GPIO_SetBits(MR1_IN2_PORT, MR1_IN2_PIN);
                break;
            case MOTOR_RIGHT_REAR:
                GPIO_ResetBits(MR2_IN1_PORT, MR2_IN1_PIN);
                GPIO_SetBits(MR2_IN2_PORT, MR2_IN2_PIN);
                break;
            case MOTOR_LEFT_FRONT:
                GPIO_ResetBits(ML1_IN1_PORT, ML1_IN1_PIN);
                GPIO_SetBits(ML1_IN2_PORT, ML1_IN2_PIN);
                break;
            case MOTOR_LEFT_REAR:
                GPIO_ResetBits(ML2_IN1_PORT, ML2_IN1_PIN);
                GPIO_SetBits(ML2_IN2_PORT, ML2_IN2_PIN);
                break;
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

void Motor_StopAll(void)
{
    Motor_SetSpeed(MOTOR_RIGHT_FRONT, 0);
    Motor_SetSpeed(MOTOR_RIGHT_REAR, 0);
    Motor_SetSpeed(MOTOR_LEFT_FRONT, 0);
    Motor_SetSpeed(MOTOR_LEFT_REAR, 0);
}

void Car_Forward(int16_t speed)
{
    Motor_SetSpeed(MOTOR_RIGHT_FRONT, -speed);
    Motor_SetSpeed(MOTOR_RIGHT_REAR, -speed);
    Motor_SetSpeed(MOTOR_LEFT_FRONT, speed);
    Motor_SetSpeed(MOTOR_LEFT_REAR, speed);
}

void Car_Backward(int16_t speed)
{
    Motor_SetSpeed(MOTOR_RIGHT_FRONT, speed);
    Motor_SetSpeed(MOTOR_RIGHT_REAR, speed);
    Motor_SetSpeed(MOTOR_LEFT_FRONT, -speed);
    Motor_SetSpeed(MOTOR_LEFT_REAR, -speed);
}

void Car_Right(int16_t speed)
{
    Motor_SetSpeed(MOTOR_RIGHT_FRONT, speed);
    Motor_SetSpeed(MOTOR_RIGHT_REAR, speed);
    Motor_SetSpeed(MOTOR_LEFT_FRONT, speed);
    Motor_SetSpeed(MOTOR_LEFT_REAR, speed);
}

void Car_Left(int16_t speed)
{
    Motor_SetSpeed(MOTOR_RIGHT_FRONT, -speed);
    Motor_SetSpeed(MOTOR_RIGHT_REAR, -speed);
    Motor_SetSpeed(MOTOR_LEFT_FRONT, -speed);
    Motor_SetSpeed(MOTOR_LEFT_REAR, -speed);
}

void USART3_IRQHandler()
{
    // 1.判断USART接收中断是否触发
    if(USART_GetITStatus(USART3, USART_IT_RXNE) == SET)
    {
        // 2.读取蓝牙下发单字节指令
        uint8_t data = USART_ReceiveData(USART3);
        
        // 3.回显数据到调试串口USART1（电脑串口助手查看收发）
        USART_SendData(USART1, data);
        while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) != SET);

        // 4.匹配安卓APP十六进制指令，执行小车动作
        switch (data)
        {
            case 0x01: // 摇杆上：前进，speed映射0~100 → PWM 0~1000
                Car_Forward((int16_t)speed * 10);
                break;
            case 0x02: // 摇杆回中：全部电机停止
                Motor_StopAll();
                break;
            case 0x03: // 摇杆下：后退
                Car_Backward((int16_t)speed * 10);
                break;
            case 0x04: // 摇杆右：原地右转
                Car_Right((int16_t)speed * 10);
                break;
            case 0x05: // 摇杆左：原地左转
                Car_Left((int16_t)speed * 10);
                break;
            case 0x06: // APP加速按键：速度+10
                speed += 10;
                if(speed > 100) speed = 100;
                break;
            case 0x07: // APP减速按键：速度-10
                if(speed < 10)
                    speed = 0;
                else
                    speed -= 10;
                break;
            case 0x08: // 超声波传感器触发（预留，可添加数据上传逻辑）
                break;
            case 0x09: // 红外循迹触发（预留）
                break;
            case 0x10: // 左转向灯（你未实现，预留函数位）
                // Car_TurnLeftLight();
                break;
            case 0x11: // 右转向灯（预留）
                // Car_TurnRightLight();
                break;
            case 0x12: // 喇叭开（预留）
                // Car_Buzzer_On();
                break;
            case 0x13: // 喇叭关（预留）
                // Car_Buzzer_Off();
                break;
            case 0x14: // 读取电池电压（预留上传）
                break;
            default: // 未知指令，不执行任何动作
                break;
        }

        // 5.清除接收中断标志位
        USART_ClearITPendingBit(USART3, USART_IT_RXNE);
    }
}

int main(void)
{
	SysTick_Init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
//	
//    Motor_GPIO_Init();
//    Motor_PWM_Init();
//		LED_Init();
	
	BLE_Init();
	
	USART3_SendStirng("ATE1\r\n");  						//打开蓝牙的回显，（给蓝牙发AT指令，然后蓝牙会回复stm32消息）
	
		delay_ms(500);
	GPIO_SetBits(LED_PWR_PORT, LED_PWR_PIN);
			USART3_SendStirng("AT+BLEMODE=9\r\n");    	//设置蓝牙位从模式
		delay_ms(500);
		USART3_SendStirng("AT+BLENAME=DWQ\r\n");
delay_ms(500);
		USART3_SendStirng("AT+BLEMODE=0\r\n");    	//设置蓝牙位从模式
		delay_ms(500);
	
    while(1)
    {
//			GPIO_SetBits(LED_PWR_PORT, LED_PWR_PIN);
//			delay_ms(3000);
//			GPIO_ResetBits(LED_PWR_PORT, LED_PWR_PIN);
//			delay_ms(3000);
    }
}
