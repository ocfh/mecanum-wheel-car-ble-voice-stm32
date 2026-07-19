#include "HC_SR04.h"

volatile float measured_distance = 0.0; // 测量距离(cm)
extern volatile uint8_t auto_avoid_flag;
extern volatile uint8_t avoid_action_flag;
int ver=0;

// ============== 超声波模块初始化 ==============
void HCSR04_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

    // 1. 使能时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);  // GPIOD时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE); // 系统配置控制器时钟
    RCC_APB1PeriphClockCmd(HCSR04_CLK, ENABLE);            // 定时器时钟

    // 2. 配置TRIG引脚（PD11 - 输出）
    GPIO_InitStructure.GPIO_Pin = TRIG_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_Init(TRIG_PORT, &GPIO_InitStructure);
    GPIO_ResetBits(TRIG_PORT, TRIG_PIN); // 初始低电平

    // 3. 配置ECHO引脚（PD10 - 输入+外部中断）
    GPIO_InitStructure.GPIO_Pin = ECHO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_Init(ECHO_PORT, &GPIO_InitStructure);

    // 4. 配置ECHO引脚外部中断（PD10 -> EXTI10）
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource10);
    EXTI_InitStructure.EXTI_Line = EXTI_Line10;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling; // 上升沿+下降沿触发
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    // 5. 配置外部中断NVIC
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // 6. 配置定时器（用于ECHO计时，1us计数周期）
    TIM_TimeBaseStructure.TIM_Period = 0xFFFF;          // 最大计数值
	//TIM_TimeBaseStructure.TIM_Period = 1000-1;          // 最大计数值
	//TIM_TimeBaseStructure.TIM_Period = 2-1;          // 最大计数值
    TIM_TimeBaseStructure.TIM_Prescaler = 84 - 1;       // 84MHz/84=1MHz -> 1us
	//TIM_TimeBaseStructure.TIM_Prescaler = 42 - 1;       // 84MHz/84=1MHz -> 1us
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(HCSR04_TIM, &TIM_TimeBaseStructure);

    // 7. 配置定时器中断NVIC
    NVIC_InitStructure.NVIC_IRQChannel = HCSR04_IRQ;	//HCSR04_IRQ  TIM4_IRQ
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // 8. 开启TIM4更新中断 + 使能定时器
	TIM_ITConfig(HCSR04_TIM, TIM_IT_Update, ENABLE); // 新增：开启更新中断
	TIM_Cmd(HCSR04_TIM, ENABLE);
}

// ============== 发送超声波触发信号 ==============
void HCSR04_Trigger(void)
{
    GPIO_SetBits(TRIG_PORT, TRIG_PIN);  // 高电平
    delay_us(10);                       // 至少10us触发脉冲
    GPIO_ResetBits(TRIG_PORT, TRIG_PIN);// 低电平
}

// ============== 获取超声波测量距离 ==============
float HCSR04_GetDistance(void)
{
    echo_flag = 0;
    measured_distance = 0.0;
    HCSR04_Trigger(); // 发送触发信号

    // 优化超时逻辑：非阻塞式等待（或缩短超时时间至60ms，HC-SR04最大测距400cm对应约23520us）
    uint32_t start_tick = SysTick->VAL; // 利用SysTick计时
    while(echo_flag != 2)
    {
        // 超时判断：约60ms（SysTick时钟为1ms滴答，需确保delay.h已初始化SysTick）
        if((start_tick - SysTick->VAL) > (60 * 84000)) // 84MHz主频，1ms=84000个时钟
        {
            echo_flag = 0;
            break;
        }
    }

    // 计算距离：时间(us) * 声速(340m/s) / 2 / 10000 = 时间 * 0.017
    if(echo_flag == 2)
    {
        uint32_t echo_duration = echo_end_time - echo_start_time;
        // 处理计数器溢出（如果TIM4计数超过65535us，补充溢出值）
        if(echo_end_time < echo_start_time)
        {
            echo_duration += 0xFFFF;
        }
        measured_distance = echo_duration * 0.017f;
        // 边界值处理
        if(measured_distance <= 0 || measured_distance > MAX_DISTANCE)
        {
            measured_distance = MAX_DISTANCE;
        }
    }
    return measured_distance;
}

// ============== 避障逻辑处理 ==============
void HCSR04_Avoidance(float distance)
{ 
	// 距离小于阈值，触发完整避障流程
	if(distance < DISTANCE_THRESHOLD && distance > 0)
	{
		avoid_action_flag = 1;  // 标记：开始执行避障动作
		if(avoid_action_flag == 1)
		{
			Motor_StopAll();
			delay_ms(2000);
			Car_Backward(600);
			delay_ms(500);
			//delay_ms(500);
		}
	}
	else 
	{
		avoid_action_flag = 0;  // 标记：避障动作执行完毕，恢复空闲
		Car_Forward(500);
		delay_ms(500);
		//Motor_StopAll();
		if(ver==0)
		{
				GPIO_SetBits(LED_PWR_PORT, LED_PWR_PIN);
				Car_Forward(500);
				delay_ms(500);
				ver=1;
		}else if(ver==1){
				GPIO_ResetBits(LED_PWR_PORT, LED_PWR_PIN);
				Car_Left(500);
				delay_ms(500);
				ver=2;
		}else if(ver==2){
				GPIO_SetBits(LED_PWR_PORT, LED_PWR_PIN);
				Car_Forward(500);
				delay_ms(500);
				ver=3;
		}else if(ver==3){
				GPIO_ResetBits(LED_PWR_PORT, LED_PWR_PIN);
				Car_Right(500);
				delay_ms(500);
				GPIO_SetBits(LED_PWR_PORT, LED_PWR_PIN);
				delay_ms(500);
				ver=4;
		}else if(ver==4){
				GPIO_ResetBits(LED_PWR_PORT, LED_PWR_PIN);
				Car_Forward(500);
				delay_ms(500);
				ver=5;
		}else if(ver==5){
				GPIO_SetBits(LED_PWR_PORT, LED_PWR_PIN);
				Car_Left(500);
				delay_ms(500);
				ver=0;
		}
	} 
}

