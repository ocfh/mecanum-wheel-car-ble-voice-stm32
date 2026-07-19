#include "delay.h"
static uint32_t fac_us = 0;  // 微秒延时倍乘数
static uint32_t fac_ms = 0;  // 毫秒延时倍乘数
void SysTick_Init(void)
{
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8); // 84MHz/8=10.5MHz，降低频率减少溢出
    fac_us = SystemCoreClock / 8 / 1000000;  // 10.5MHz -> fac_us=10.5 → 取整为10（或用浮点）
    // 兼容 24 位 LOAD 最大值：0xFFFFFF / 10 = 1677721.5 us ≈ 1677 ms
    fac_ms = fac_us * 1000;
}
// 修正后的微秒延时（避免溢出）
void delay_us(uint32_t us)
{
    uint32_t temp;
    uint32_t reload = us * fac_us;
    
    // 限制 LOAD 不超过 24 位
    if(reload > 0xFFFFFF) reload = 0xFFFFFF;
    
    SysTick->LOAD = reload;
    SysTick->VAL = 0x00;
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
    do {
        temp = SysTick->CTRL;
    } while ((temp & SysTick_CTRL_ENABLE_Msk) && !(temp & SysTick_CTRL_COUNTFLAG_Msk));
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
    SysTick->VAL = 0x00;
}

// 修正后的毫秒延时（分段延时，避免单次溢出）
void delay_ms(uint32_t ms)
{
    // 分段处理：每次最多延时 1000ms，避免 reload 溢出
    while(ms > 0)
    {
        uint32_t temp;
        uint32_t curr_ms = (ms > 1000) ? 1000 : ms;
        uint32_t reload = curr_ms * fac_ms;
        
        if(reload > 0xFFFFFF) reload = 0xFFFFFF;
        
        SysTick->LOAD = reload;
        SysTick->VAL = 0x00;
        SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
        do {
            temp = SysTick->CTRL;
        } while ((temp & SysTick_CTRL_ENABLE_Msk) && !(temp & SysTick_CTRL_COUNTFLAG_Msk));
        SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
        SysTick->VAL = 0x00;
        
        ms -= curr_ms;
    }
}






























