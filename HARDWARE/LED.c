#include "LED.h"

// ============== 原有LED初始化 ==============
void LED_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    GPIO_InitStructure.GPIO_Pin   = LED_PWR_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
    GPIO_Init(LED_PWR_PORT, &GPIO_InitStructure);
    GPIO_ResetBits(LED_PWR_PORT, LED_PWR_PIN);
}

// LED 快速闪烁3次
void LED_Flash_3Times(void)
{
    for(uint8_t i=0; i<3; i++)
    {
        GPIO_SetBits(LED_PWR_PORT, LED_PWR_PIN);
        delay_ms(150);
        GPIO_ResetBits(LED_PWR_PORT, LED_PWR_PIN);
        delay_ms(150);
    }
}
