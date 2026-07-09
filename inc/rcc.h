#ifndef RCC_H
#define RCC_H

#include <stdint.h>

// memory mapped registers
#define FLASH_ACR (*(volatile uint32_t *)0x40023C00)
#define APB1ENR (*(volatile uint32_t *)(0x40023800 + 0x40)) // base addr + offset
#define PWR_CR (*(volatile uint32_t *)0x40007000)
#define PWR_CSR (*(volatile uint32_t *)(0x40007000 + 0x04))
#define RCC_CR (*(volatile uint32_t *)(0x40023800))
#define RCC_PLLCFGR (*(volatile uint32_t *)(0x40023800 + 0x04))
#define RCC_CFGR (*(volatile uint32_t *)(0x40023800 + 0x08))

void SystemClockConfig(void);


#endif // RCC_H
