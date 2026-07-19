#include "stm32f4xx.h"                  // Device header
#include "Motor.h"
#include "LED.h"
#include "SU03T.h"


//语音识别模块初始化
//  PC12:  输出高电平打开语音识别模块供电
//  PC10： MCU_TX   VOI_RX    UART4_TX
//  PC11:  MCU_RX   VOI_TX    UART4_RX
void  Su03t_Init()
{
		//1.打开时钟GPIOC、UART4
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
	
	
		//2.初始化PC12， 
		GPIO_InitTypeDef  GPIO_InitStruct;  //包含了GPIO的配置信息
	
		GPIO_InitStruct.GPIO_Mode   = GPIO_Mode_OUT;     //输出模式
		GPIO_InitStruct.GPIO_OType  = GPIO_OType_PP;     //推挽输出
		GPIO_InitStruct.GPIO_Pin    = GPIO_Pin_12;  		 //引脚编号
		//GPIO_InitStruct.GPIO_PuPd   = ;   						 //上下拉模式
		GPIO_InitStruct.GPIO_Speed  = GPIO_High_Speed;   //引脚速度
	
		GPIO_Init(GPIOC, &GPIO_InitStruct);	
		
		//3.PC12输出高电平打开语音识别模块供电
		GPIO_SetBits(GPIOC, GPIO_Pin_12);
	
	
		//4.配置PC10、PC11
		GPIO_InitStruct.GPIO_Mode   = GPIO_Mode_AF;     				//复用模式
		GPIO_InitStruct.GPIO_OType  = GPIO_OType_PP;     				//推挽输出
		GPIO_InitStruct.GPIO_Pin    = GPIO_Pin_10| GPIO_Pin_11; 	//引脚编号
		//GPIO_InitStruct.GPIO_PuPd   = ;   						 				//上下拉模式
		GPIO_InitStruct.GPIO_Speed  = GPIO_High_Speed;   				//引脚速度
	
		GPIO_Init(GPIOC, &GPIO_InitStruct);
		
		
		//5.配置并初始化UART4
		USART_InitTypeDef  USART_InitStruct;
	
		USART_InitStruct.USART_BaudRate              = 9600;      										//波特率
		USART_InitStruct.USART_HardwareFlowControl   = USART_HardwareFlowControl_None;//无硬件流控(防止数据收发的速率不一样)
		USART_InitStruct.USART_Mode                  = USART_Mode_Rx| USART_Mode_Tx;  //收发模式
		USART_InitStruct.USART_Parity                = USART_Parity_No;     					//无校验位
		USART_InitStruct.USART_StopBits              = USART_StopBits_1;    					//停止位长度
		USART_InitStruct.USART_WordLength            = USART_WordLength_8b; 					//数据位的长度
	
		USART_Init(UART4, &USART_InitStruct);		
		
		
		//6.配置USART3中断通道
		USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);
		
		
		//7.配置并初始化NVIC（中断控制器）（NVIC是位于内核的，所以去misc.h中查找，该文件放的是和内核相关的库函数）
		NVIC_InitTypeDef   NVIC_InitStruct;
		
		NVIC_InitStruct.NVIC_IRQChannel                    = UART4_IRQn; //中断通道
		NVIC_InitStruct.NVIC_IRQChannelCmd                 = ENABLE;			//使能
		NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority  = 0; 					//抢占优先级（0-3）  数值越低，优先级越高
		NVIC_InitStruct.NVIC_IRQChannelSubPriority         = 0; 					//响应优先级（0-3）  数值越低，优先级越高
		
		NVIC_Init(&NVIC_InitStruct);
		
		//8.使能USART3
		USART_Cmd(UART4, ENABLE);
		
		
		//9.把UART4复用到GPIO引脚上
		GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_UART4);
		GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_UART4);
			
}



//中断任务函数
//没有参数、没有返回值
//名字是固定的，因为该函数由系统自动调用（名字去CMSIS文件夹中的.s文件中查找）
volatile int16_t RUN1_SPEED = 300;  // 小车默认运行速度

//stm32接收语音识别模块发过来的数据     
void  UART4_IRQHandler()
{
		//1.判断USART中断是否触发
		if(USART_GetITStatus(UART4, USART_IT_RXNE)==SET)
		{
	
				//2.接收数据
				uint8_t data = USART_ReceiveData(UART4);
			
//			USART_SendData(USART1,rx_data);
		switch(data)
		{
			case 0x0A: Car_Forward(RUN1_SPEED); break;    // 前进
			case 0xA0: Car_Backward(RUN1_SPEED); break;   // 后退
			case 0x43: Car_Left(RUN1_SPEED); break;       // 左转
			case 0x44: Car_Right(RUN1_SPEED); break;      // 右转
			//case 0x0A: Motor_StopAll(); break;           // 停止
			//case 'D': RUN1_SPEED+=50; break;      		// 加速
			case 'U': RUN1_SPEED-=50; break;           	// 减速
			case 'O': GPIO_SetBits(LED_PWR_PORT, LED_PWR_PIN); break; // 开灯
			case 'I': GPIO_ResetBits(LED_PWR_PORT, LED_PWR_PIN); break;// 关灯
			case 'S': LED_Flash_3Times(); break;        // 快速闪烁3次
			
//			// ========== 避障指令 A 核心补充 ==========
//			case 'A': 
//			{
//				auto_avoid_flag = !auto_avoid_flag;
//				if(auto_avoid_flag)
//				{
//					//USART3_Send_String((uint8_t*)"Auto Avoid ON\r\n");
//				}
//				else
//				{
//					Motor_StopAll();       // 关闭避障，立即停车
//					avoid_action_flag = 0; // 清空动作状态
//					//USART3_Send_String((uint8_t*)"Auto Avoid OFF\r\n");
//				}
//				break;
//			}
			default: break;
				//5.清除USART中断标志位
			USART_ClearITPendingBit(UART4, USART_IT_RXNE);
		}
	}
}

























