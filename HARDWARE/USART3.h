#ifndef __USART3_H__
#define __USART3_H__
#include "stm32f4xx.h"
#include <stdio.h>
#include <string.h>

#define USART3_BAUDRATE 115200    // 蓝牙模块默认波特率 9600
#define USART3_RX_BUF_LEN 8     // 串口接收缓冲区


void USART3_Init(uint32_t baud);
void USART3_Send_String(uint8_t *str);
//void USART3_Send_T(float dist);
//void USART3_Send_H(float dist);

#endif

