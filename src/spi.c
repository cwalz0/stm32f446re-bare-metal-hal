#include "spi.h"
#include "gpio.h"
#include "rcc.h"
#include <stdint.h>

void SPI2_Init(void){
	RCC_EnableClock(AHB1ENR_OFFSET, 1); // Turn on GPIO B
	
	RCC_EnableClock(APB1ENR_OFFSET, 14); // Turn on SPI2

	GPIO_InitTypeDef SPI2_Init = {0};
	SPI2_Init.Mode = GPIO_MODE_ALT;
	SPI2_Init.Alternate = 5;  // AF5 = SPI2
	SPI2_Init.Speed = 3;

	// configure PB13 (SCK)
	SPI2_Init.Pin = 13;
	GPIO_Init(GPIOB, &SPI2_Init);
	// configure PB9 (MOSI)
	//SPI2_Init.Pin = 9;
	//GPIO_Init(GPIOB, &SPI2_Init);
	// configure PB15 (MOSI)
	SPI2_Init.Pin = 15;
	GPIO_Init(GPIOB, &SPI2_Init);

	// configure spi

	// master mode (MSTR = 1)
	*(volatile uint32_t *)(SPI2_BASE + SPI_CR1_OFFSET) |= (1U << 2);
	

	// baud rate 010 ~ fpclk/8
	*(volatile uint32_t *)(SPI2_BASE + SPI_CR1_OFFSET) &= ~(7U << 3); // clear 3-5
	*(volatile uint32_t *)(SPI2_BASE + SPI_CR1_OFFSET) |= (2U << 3); // set 010
	
	// SSI (enabled 1)
	*(volatile uint32_t *)(SPI2_BASE + SPI_CR1_OFFSET) |= (1U << 8);
	// SSM (enabled 1)
	*(volatile uint32_t *)(SPI2_BASE + SPI_CR1_OFFSET) |= (1U << 9);
	
	// SPI (enable 1)
	*(volatile uint32_t *)(SPI2_BASE + SPI_CR1_OFFSET) |= (1U << 6);

	return;
}

void SPI2_WriteByte(uint8_t data){

	// wait for an empty transmit buffer (1)
	while ((*(volatile uint32_t *)(SPI2_BASE + SPI_SR_OFFSET) & (1U << 1)) == 0){
		continue;
	}

	*(volatile uint8_t *)(SPI2_BASE + SPI_DR_OFFSET) = data;

	while ((*(volatile uint32_t *)(SPI2_BASE + SPI_SR_OFFSET) &  (1U << 1)) == 0){
		continue;
	}
	// wait for SPI to not be busy (0)
	while ((*(volatile uint32_t *)(SPI2_BASE + SPI_SR_OFFSET) & (1U << 7)) != 0){
		continue;
	}
}
