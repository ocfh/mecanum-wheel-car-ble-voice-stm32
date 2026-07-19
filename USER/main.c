#include "stm32f4xx.h"
#include "delay.h"
#include "DHT11.h"
#include "LED.h"
#include "Motor.h"
#include "USART1.h"
#include "USART3.h"
#include "HC_SR04.h"
#include "SU03T.h"



//----------------串口1变量------------------
//extern uint8_t usart_rx_buf[USART_RX_BUF_LEN];
//extern uint8_t usart_rx_cnt;
//------------------ End --------------------

//-----------------超声波相关变量--------------------
extern volatile uint8_t auto_avoid_flag;
////自动避障总开关
//volatile uint8_t auto_avoid_flag = 0;
//// 避障动作状态：0=空闲  1=正在执行避障动作
//volatile uint8_t avoid_action_flag = 0;
//--------------------- End -------------------------

// --------------- 函数声明 ---------------

//void Process_Command(uint8_t cmd); // 指令解析处理


//// ============== 指令解析处理函数 ==============
//void Process_Command(uint8_t cmd)
//{
//    const int16_t RUN_SPEED = 500;  // 小车默认运行速度
//    switch(cmd)
//    {
//        case 'G': Car_Forward(RUN_SPEED); break;    // 前进
//        case 'B': Car_Backward(RUN_SPEED); break;   // 后退
//        case 'L': Car_Left(RUN_SPEED); break;       // 左转
//        case 'R': Car_Right(RUN_SPEED); break;      // 右转
//        case 'P': Motor_StopAll(); break;           // 停止
//        case 'O': GPIO_SetBits(LED_PWR_PORT, LED_PWR_PIN); break; // 开灯
//        case 'I': GPIO_ResetBits(LED_PWR_PORT, LED_PWR_PIN); break;// 关灯
//        case 'S': LED_Flash_3Times(); break;        // 快速闪烁3次
//        
//        // ========== 避障指令 A 核心补充 ==========
//        case 'A': 
//		{
//			auto_avoid_flag = !auto_avoid_flag;
//			if(auto_avoid_flag)
//			{
//				USART1_Send_String((uint8_t*)"Auto Avoid ON\r\n");
//			}
//			else
//			{
//				Motor_StopAll();       // 关闭避障，立即停车
//				avoid_action_flag = 0; // 清空动作状态
//				USART1_Send_String((uint8_t*)"Auto Avoid OFF\r\n");
//			}
//			break;
//		}
//        default: break;
//    }
//}

// ============== 主函数 ==============
int main(void)
{
    // 1. 先初始化SysTick（必须最先执行，否则延时失效）
    SysTick_Init();
    
    // 2. 硬件初始化（顺序调整，避免冲突）
    LED_Init();                // 先初始化LED，确保闪烁可见
    HCSR04_Init();
	
    USART1_Init(USART_BAUDRATE);
	
    USART3_Init(USART3_BAUDRATE);
	
	
	USART3_Send_String("ATE1\r\n");
	delay_ms(2000);
	USART3_Send_String("AT+BLEMODE=0\r\n");
	delay_ms(2000);
	
	USART3_Send_String("AT+GMR\r\n");
	delay_ms(1000);
//	USART3_Send_String("AT+BLEMODE=9\r\n");
//	delay_ms(1000);
//	
//	USART3_Send_String("AT+BLENAME=F4DWQ\r\n");
//	
//	delay_ms(1000);
	Su03t_Init();
	//USART3_Init(USART3_BAUDRATE);
	Motor_GPIO_Init();
    Motor_PWM_Init();
//    usart_rx_cnt = 0;
//    memset(usart_rx_buf, 0, USART_RX_BUF_LEN);

    // 3. LED闪烁3次（此时延时函数已生效）
	//跳过蓝牙回复
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	//USART_Cmd(USART3, ENABLE);
    LED_Flash_3Times();  // 开机提示
//	static uint32_t time_TH = 0;
	
    while(1)
    {
		
//		time_TH++;
//		if(time_TH >= 100)  // 100ms 检测一次
//		{
//			//读取模块数据
//				DHT11_Read_Data();
//				//显示读取后的温度数据
//				USART1_Send_T(Get_temperature() );
//				USART1_Send_H(Get_humidity());
//				time_TH = 0;
//		}
//		delay_ms(10);
		
//        printf("temperature = %.2f\r\n", Get_temperature() );
//        //显示读取后的湿度数据
//        printf("humidity = %.2f\r\n", Get_humidity() );
		
        // 串口指令处理（增加防呆，只处理有效指令）
//        if(usart_rx_cnt > 0)
//        {
//            Process_Command(usart_rx_buf[0]);	
//            // 清除缓冲区（优化：只清已处理的字节，避免漏指令）
//            for(uint8_t i=0; i<usart_rx_cnt; i++)
//                usart_rx_buf[i] = 0;
//            usart_rx_cnt = 0;
//        }
        // 自动避障：开启状态下每100ms检测一次
		static uint32_t time_cnt = 0;
		if(auto_avoid_flag == 1)	//自动避障标志位
		{
			time_cnt++;
			if(time_cnt >= 100)  // 100ms 检测一次
			{
				float distance = HCSR04_GetDistance();
				HCSR04_Avoidance(distance);
				time_cnt = 0;
			}
			delay_ms(10);
		}
    }
}



