#include "stm32f4xx.h"                  // Device header
#include "BLE.h"
#include "Motor.h"




//PD8  :  MCU_TX  :  USART3_TX
//PD9  :  MCU_RX  :  USART3_RX
//PB15 :  输出高电平打开蓝牙供电
//蓝牙初始化
void BLE_Init()
{
		//1.打开GPIOB、GPIOD、USART3时钟
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOD, ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE );	

	
		//2.配置GPIOB端口
		GPIO_InitTypeDef  GPIO_InitStruct;  //包含了GPIO的配置信息
	
		GPIO_InitStruct.GPIO_Mode   = GPIO_Mode_OUT;     //输出模式
		GPIO_InitStruct.GPIO_OType  = GPIO_OType_PP;     //推挽输出
		GPIO_InitStruct.GPIO_Pin    = GPIO_Pin_15;  		 //引脚编号
		//GPIO_InitStruct.GPIO_PuPd   = ;   						 //上下拉模式
		GPIO_InitStruct.GPIO_Speed  = GPIO_High_Speed;   //引脚速度	

		GPIO_Init(GPIOB, &GPIO_InitStruct);
	
		//3.让PB15输出高电平打开蓝牙供电
		GPIO_SetBits(GPIOB, GPIO_Pin_15);
	
	
		//4.配置GPIOD端口
		GPIO_InitStruct.GPIO_Mode   = GPIO_Mode_AF;     				//复用模式
		GPIO_InitStruct.GPIO_OType  = GPIO_OType_PP;     				//推挽输出
		GPIO_InitStruct.GPIO_Pin    = GPIO_Pin_9 | GPIO_Pin_8; 	//引脚编号
		//GPIO_InitStruct.GPIO_PuPd   = ;   						 				//上下拉模式
		GPIO_InitStruct.GPIO_Speed  = GPIO_High_Speed;   				//引脚速度
	
		GPIO_Init(GPIOD, &GPIO_InitStruct);
		
		
		//5.配置并初始化USART3
		USART_InitTypeDef  USART_InitStruct;
	
		USART_InitStruct.USART_BaudRate              = 115200;      									//波特率
		USART_InitStruct.USART_HardwareFlowControl   = USART_HardwareFlowControl_None;//无硬件流控(防止数据收发的速率不一样)
		USART_InitStruct.USART_Mode                  = USART_Mode_Rx| USART_Mode_Tx;  //收发模式
		USART_InitStruct.USART_Parity                = USART_Parity_No;     					//无校验位
		USART_InitStruct.USART_StopBits              = USART_StopBits_1;    					//停止位长度
		USART_InitStruct.USART_WordLength            = USART_WordLength_8b; 					//数据位的长度
	
		USART_Init(USART3, &USART_InitStruct);		
		
		
		//6.配置USART3中断通道
		USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
		
		
		//7.配置并初始化NVIC（中断控制器）（NVIC是位于内核的，所以去misc.h中查找，该文件放的是和内核相关的库函数）
		NVIC_InitTypeDef   NVIC_InitStruct;
		
		NVIC_InitStruct.NVIC_IRQChannel                    = USART3_IRQn; //中断通道
		NVIC_InitStruct.NVIC_IRQChannelCmd                 = ENABLE;			//使能
		NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority  = 0; 					//抢占优先级（0-3）  数值越低，优先级越高
		NVIC_InitStruct.NVIC_IRQChannelSubPriority         = 0; 					//响应优先级（0-3）  数值越低，优先级越高
		
		NVIC_Init(&NVIC_InitStruct);
		
		//8.使能USART3
		USART_Cmd(USART3, ENABLE);
		
		
		//9.把USART1复用到GPIO引脚上
		GPIO_PinAFConfig(GPIOD, GPIO_PinSource9, GPIO_AF_USART3);
		GPIO_PinAFConfig(GPIOD, GPIO_PinSource8, GPIO_AF_USART3);
		
}



//USART3发送字符串
//stm32给蓝牙发送字符串
void USART3_SendStirng(uint8_t *string)
{
			while(*string!='\0')
			{
					USART_SendData(USART3,*string++);  //一次最大发16位
				
					//等待发送完成(发送数据寄存器位空就代表发送完成)
					while(USART_GetFlagStatus(USART3, USART_FLAG_TXE)!=1);		
			}
}



//中断任务函数
//没有参数、没有返回值
//名字是固定的，因为该函数由系统自动调用（名字去CMSIS文件夹中的.s文件中查找）

//stm32接收蓝牙发过来的数据     


























