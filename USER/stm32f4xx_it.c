/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Templates/stm32f4xx_it.c 
  * @author  MCD Application Team
  * @version V1.4.0
  * @date    04-August-2014
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_it.h"
//#include "USART1.h"
#include "USART3.h"
#include "delay.h"
#include "HC_SR04.h"
//#include "main.h"

/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
  //TimingDelay_Decrement();
}

/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f4xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 
//uint8_t usart_rx_buf[USART_RX_BUF_LEN];
//uint8_t usart_rx_cnt = 0;






//// USART1接受中断服务函数（正确清除标志）
//void USART1_IRQHandler(void)
//{
//    //uint8_t rx_data = 0;
//    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
//    {
//        uint8_t rx_data = USART_ReceiveData(USART1);  // 先读数据，清除RXNE标志
//        usart_rx_buf[usart_rx_cnt++] = rx_data;
//		
//		
////	switch(rx_data)
////    {
////        case 'G': Car_Forward(RUN_SPEED); break;    // 前进
////        case 'B': Car_Backward(RUN_SPEED); break;   // 后退
////        case 'L': Car_Left(RUN_SPEED); break;       // 左转
////        case 'R': Car_Right(RUN_SPEED); break;      // 右转
////        case 'P': Motor_StopAll(); break;           // 停止
////        case 'O': GPIO_SetBits(LED_PWR_PORT, LED_PWR_PIN); break; // 开灯
////        case 'I': GPIO_ResetBits(LED_PWR_PORT, LED_PWR_PIN); break;// 关灯
////        case 'S': LED_Flash_3Times(); break;        // 快速闪烁3次
////        
////        // ========== 避障指令 A 核心补充 ==========
////        case 'A': 
////		{
////			auto_avoid_flag = !auto_avoid_flag;
////			if(auto_avoid_flag)
////			{
////				USART1_Send_String((uint8_t*)"Auto Avoid ON\r\n");
////			}
////			else
////			{
////				Motor_StopAll();       // 关闭避障，立即停车
////				avoid_action_flag = 0; // 清空动作状态
////				USART1_Send_String((uint8_t*)"Auto Avoid OFF\r\n");
////			}
////			break;
////		}
////        default: break;
////    }
//		
//        if(usart_rx_cnt >= USART_RX_BUF_LEN) 
//            usart_rx_cnt = 0;
//		
//        USART_ClearITPendingBit(USART1, USART_IT_RXNE); // 显式清除中断挂起位
//		//USART_SendData(USART1,rx_data);
//    }
//    // 防止溢出中断导致卡死
//    if(USART_GetFlagStatus(USART1, USART_FLAG_ORE) != RESET)
//    {
//        USART_ClearFlag(USART1, USART_FLAG_ORE);
//        USART_ReceiveData(USART1);
//    }
//}

//自动避障总开关
volatile uint8_t auto_avoid_flag = 0;
// 避障动作状态：0=空闲  1=正在执行避障动作
volatile uint8_t avoid_action_flag = 0;

volatile int16_t RUN_SPEED = 300;  // 小车默认运行速度
uint8_t usart3_rx_buf[USART3_RX_BUF_LEN];
uint8_t usart3_rx_cnt = 0;

// USART3接受中断服务函数（正确清除标志）
void USART3_IRQHandler(void)
{

    if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
    {
        uint8_t rx_data = USART_ReceiveData(USART3);  // 先读数据，清除RXNE标志
		USART_SendData(USART1,rx_data);
		switch(rx_data)
		{
			case 'G': Car_Forward(RUN_SPEED); break;    // 前进
			case 'B': Car_Backward(RUN_SPEED); break;   // 后退
			case 'L': Car_Left(RUN_SPEED); break;       // 左转
			case 'R': Car_Right(RUN_SPEED); break;      // 右转
			case 'P': Motor_StopAll(); break;           // 停止
			case 'D': RUN_SPEED+=50; break;      		// 加速
			case 'U': RUN_SPEED-=50; break;           	// 减速
			case 'O': GPIO_SetBits(LED_PWR_PORT, LED_PWR_PIN); break; // 开灯
			case 'I': GPIO_ResetBits(LED_PWR_PORT, LED_PWR_PIN); break;// 关灯
			case 'S': LED_Flash_3Times(); break;        // 快速闪烁3次
			
			// ========== 避障指令 A 核心补充 ==========
			case 'A': 
			{
				auto_avoid_flag = !auto_avoid_flag;
				if(auto_avoid_flag)
				{
					//USART3_Send_String((uint8_t*)"Auto Avoid ON\r\n");
				}
				else
				{
					Motor_StopAll();       // 关闭避障，立即停车
					avoid_action_flag = 0; // 清空动作状态
					//USART3_Send_String((uint8_t*)"Auto Avoid OFF\r\n");
				}
				break;
			}
			default: break;
		}
		
		while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) != 1)
		
        USART_ClearITPendingBit(USART3, USART_IT_RXNE); // 显式清除中断挂起位
		
    }
    // 防止溢出中断导致卡死
    if(USART_GetFlagStatus(USART3, USART_FLAG_ORE) != RESET)
    {
        USART_ClearFlag(USART3, USART_FLAG_ORE);
        USART_ReceiveData(USART3);
    }
}


//// USART1接受中断服务函数（正确清除标志）
//void USART3_IRQHandler(void)
//{
//    //uint8_t rx_data = 0;
//    if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
//    {
//        uint8_t rx_data = USART_ReceiveData(USART3);  // 先读数据，清除RXNE标志
//		//USART_SendData(USART1,rx_data);
//		switch(rx_data)
//		{
//			case 'G': Car_Forward(RUN_SPEED); break;    // 前进
//			case 'B': Car_Backward(RUN_SPEED); break;   // 后退
//			case 'L': Car_Left(RUN_SPEED); break;       // 左转
//			case 'R': Car_Right(RUN_SPEED); break;      // 右转
//			case 'P': Motor_StopAll(); break;           // 停止
//			case 'O': GPIO_SetBits(LED_PWR_PORT, LED_PWR_PIN); break; // 开灯
//			case 'I': GPIO_ResetBits(LED_PWR_PORT, LED_PWR_PIN); break;// 关灯
//			case 'S': LED_Flash_3Times(); break;        // 快速闪烁3次
//			
//			// ========== 避障指令 A 核心补充 ==========
//			case 'A': 
//			{
//				auto_avoid_flag = !auto_avoid_flag;
//				if(auto_avoid_flag)
//				{
//					USART3_Send_String((uint8_t*)"Auto Avoid ON\r\n");
//				}
//				else
//				{
//					Motor_StopAll();       // 关闭避障，立即停车
//					avoid_action_flag = 0; // 清空动作状态
//					USART3_Send_String((uint8_t*)"Auto Avoid OFF\r\n");
//				}
//				break;
//			}
//			default: break;
//		}
//        //usart3_rx_buf[usart3_rx_cnt++] = rx_data;
//		
//		while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) != 1)
//		
////        if(usart3_rx_cnt >= USART_RX_BUF_LEN) 
////            usart3_rx_cnt = 0;
//		
//        USART_ClearITPendingBit(USART3, USART_IT_RXNE); // 显式清除中断挂起位
//		
//    }
//    // 防止溢出中断导致卡死
//    if(USART_GetFlagStatus(USART3, USART_FLAG_ORE) != RESET)
//    {
//        USART_ClearFlag(USART3, USART_FLAG_ORE);
//        USART_ReceiveData(USART3);
//    }
//}
//超声波变量
volatile uint32_t echo_start_time = 0;  // ECHO上升沿时间戳
volatile uint32_t echo_end_time = 0;    // ECHO下降沿时间戳
volatile uint8_t echo_flag = 0;         // ECHO捕获标志

// ============== ECHO引脚外部中断服务函数 ==============
void EXTI15_10_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line10) != RESET)
    {
        // 增加消抖（防止电平抖动导致误触发）
        delay_us(2); 
        if(GPIO_ReadInputDataBit(ECHO_PORT, ECHO_PIN) == 1)
        {
            // 上升沿：记录开始时间，重置计数器
            TIM_SetCounter(HCSR04_TIM, 0); // 先重置计数器，再读值
            echo_start_time = TIM_GetCounter(HCSR04_TIM);
            echo_flag = 1;
        }
        else
        {
            // 下降沿：记录结束时间
            echo_end_time = TIM_GetCounter(HCSR04_TIM);
            echo_flag = 2; // 测距完成标志
        }
        EXTI_ClearITPendingBit(EXTI_Line10);
    }
}

//volatile uint32_t time = 0;         // ECHO捕获标志
// ============== 定时器4中断服务函数（超时保护） ==============
void TIM4_IRQHandler(void)
{
    if(TIM_GetITStatus(HCSR04_TIM, TIM_IT_Update) != RESET)
    {
//		time++;
//		if(time>1000000)
//		{
//			GPIO_ToggleBits(LED_PWR_PORT,LED_PWR_PIN);
//			time = 0;
//		}
        echo_flag = 0; // 超时，重置标志
        TIM_ClearITPendingBit(HCSR04_TIM, TIM_IT_Update);
    }
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
