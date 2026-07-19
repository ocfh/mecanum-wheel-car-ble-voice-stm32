#include "USART3.h"

// ============== USART1 初始化 + 中断接收 ==============

void USART3_Init(uint32_t baud)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    USART_InitTypeDef USART_InitStruct;
    NVIC_InitTypeDef NVIC_InitStruct;

    // 1. 先使能时钟（严格时序）
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD|RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	
	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_15;
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    //GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_DOWN;
    GPIO_Init(GPIOB, &GPIO_InitStruct);
    GPIO_SetBits(GPIOB, GPIO_Pin_15);
    //RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART3, ENABLE);
   // RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

    // 2. 配置PA9(TX)/PA10(RX)复用
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOD, &GPIO_InitStruct);

    // 3. 复用映射（必须单独配置，避免映射失效）
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource8, GPIO_AF_USART3);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource9, GPIO_AF_USART3);

    // 4. 串口参数配置
    USART_InitStruct.USART_BaudRate = baud;
    USART_InitStruct.USART_WordLength = USART_WordLength_8b;
    USART_InitStruct.USART_StopBits = USART_StopBits_1;
    USART_InitStruct.USART_Parity = USART_Parity_No;
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART3, &USART_InitStruct);

    // 5. 清除所有中断标志（关键！避免初始化时中断卡死）
    USART_ClearFlag(USART3, USART_FLAG_TC | USART_FLAG_RXNE | USART_FLAG_IDLE);
    USART_ClearITPendingBit(USART3, USART_IT_RXNE);

    // 6. 配置中断（降低优先级，避免与避障冲突）
    //USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    NVIC_InitStruct.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;  // G于EXTI/TIM4的1级
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);

    // 7. 使能串口
    USART_Cmd(USART3, ENABLE);
}


// 新增：串口1发送字符串函数（需实现）
void USART3_Send_String(uint8_t *str)
{
    while(*str)
    {
		USART_SendData(USART3, *str++);
        while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
       
    }
}

////// 新增：串口1发送距离函数
//void USART1_Send_H(float dist)
//{
//    char buf[32];
//    sprintf(buf, "H: %.2f \r\n", dist);
//    USART1_Send_String((uint8_t*)buf);
//}
//void USART1_Send_T(float dist)
//{
//    char buf[32];
//    sprintf(buf, "T: %.2f \r\n", dist);
//    USART1_Send_String((uint8_t*)buf);
//}

