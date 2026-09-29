#include "rcc.h"
#include <stdint.h>


void SystemClockConfig(void){
	// enable instruction cache, data cache prefetch (assign 1 to bits 9. 10, 8)
	// and set latency to 5 WS (0101) ~ 5
	//
	FLASH_ACR |= (1 << 9) | (1 << 10) | (1 << 8);

	FLASH_ACR &= ~(0xFU << 0); // clear bits 0-3
	FLASH_ACR |= (5U << 0);    // set to 5 WS


	// enable power interface clock
	RCC_APB1ENR |= (1 << 28);

	// set PLLP = 2 (0,0) 16, 17
	RCC_PLLCFGR &= ~(3 << 16);

	// set PLLN to 180 (10110100) + 6 
	RCC_PLLCFGR &= ~(0x1FF << 6);
	RCC_PLLCFGR |= (180 << 6);

	// set PLLM to 8 (..01000)
	RCC_PLLCFGR &= ~(0x3F << 0);
	RCC_PLLCFGR |= (8 << 0);

	// set PLLON 
	RCC_CR |= (1 << 24);

	// enable over-drive switching set ODEN TO 1
	PWR_CR |= (1 << 16);

	while ((PWR_CSR & (1 << 16)) == 0) { continue; }
	
	// enable over-drive switching
	PWR_CR |= (1 << 17);
	while ((PWR_CSR & (1 << 17)) == 0) { continue; }

	// check if Main PLL clock ready (flag)
	while ((RCC_CR & (1 << 25)) == 0){ continue; }

	// set APB1 prescalar to /4 (bits 10-12 (101))
	RCC_CFGR &= ~(7U << 10);
	RCC_CFGR |= (5U << 10);

	// set APB2 prescalar to /2 (bits 13-15 (100))
	RCC_CFGR &= ~(7U << 13);
	RCC_CFGR |= (4U << 13);

	// switch system clock to PLL_P (1, 0)
	RCC_CFGR &= ~(3 << 0);
	RCC_CFGR |= (1 << 1);

	while ((RCC_CFGR & (3 << 2)) != (2 << 2)){ continue; }
	
}

void RCC_EnableClock(uint32_t bus_offset, uint8_t bit_position){
	*(volatile uint32_t *)(RCC_BASE + bus_offset) |= (1U << bit_position);
}
