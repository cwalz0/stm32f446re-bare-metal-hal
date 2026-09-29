#include "systick.h"
#include <stdint.h>

static volatile uint32_t tick_ms = 0;

void SysTick_Handler(void){
	tick_ms++;
}

uint32_t SysTick_GetMillis(void){
	return tick_ms;
}


void SysTick_Init(void){
	*(volatile uint32_t *)(SYSTICK_BASE + STK_LOAD) = ((180000U - 1) << 0); // 180k - 1 physical ticks in a ms ( ~ 180MHZ system clock speed)
	
	*(volatile uint32_t *)(SYSTICK_BASE + STK_VAL) = (1U << 0); // "a write of any value clears the field to 0" (i assume this clears the current value then?)
	
	*(volatile uint32_t *)(SYSTICK_BASE + STK_CTRL) = (1U << 0) | (1U << 1) | (1U << 2); // enable counter (bit 0) enable processor clock (bit 2)

}

void Delay_ms(uint32_t ms){

	for (uint32_t i = 0; i < ms; i++){
		while ((*(volatile uint32_t *)(SYSTICK_BASE + STK_CTRL) & (1U << 16)) == 0) {
			continue;
		}
	}
}




