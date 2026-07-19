#ifndef __LED_H__
#define __LED_H__
#include  "stm32f4xx.h"
#include  "delay.h"

#define LED_PWR_PIN    GPIO_Pin_13
#define LED_PWR_PORT   GPIOB


void LED_Init(void);
void LED_Flash_3Times(void);   // LEDøÏÀŸ…¡À∏3¥Œ

#endif

