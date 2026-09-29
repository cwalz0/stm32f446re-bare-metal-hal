#ifndef SYSTICK_H
#define SYSTICK_H

#include <stdint.h>

#define SYSTICK_BASE 0xE000E010U

#define STK_CTRL 0x00U
#define STK_LOAD 0x04U
#define STK_VAL 0x08U

void SysTick_Init(void);
void SysTick_Handler(void);
uint32_t SysTick_GetMillis(void);
void Delay_ms(uint32_t ms);

#endif

