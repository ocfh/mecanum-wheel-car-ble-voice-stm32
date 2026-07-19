#ifndef __USART1_H__
#define __USART1_H__
#include "stm32f4xx.h"
#include <stdio.h>
#include <string.h>

#define USART_BAUDRATE 115200    // 蓝牙模块默认波特率 9600
#define USART_RX_BUF_LEN 8     // 串口接收缓冲区


void USART1_Init(uint32_t baud);
void USART1_Send_String(uint8_t *str);
void USART1_Send_T(float dist);
void USART1_Send_H(float dist);

#endif

