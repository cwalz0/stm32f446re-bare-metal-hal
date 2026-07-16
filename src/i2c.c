#include "i2c.h"
#include "gpio.h"
#include "rcc.h"
#include <stdint.h>
#include <sys/types.h>

void I2C_Init(void){
	RCC_EnableClock(AHB1ENR_OFFSET, 1); // power GPIO B
	RCC_EnableClock(APB1ENR_OFFSET, 21); // power IC21
	
	GPIO_InitTypeDef I2C_INIT = {0};

	I2C_INIT.Mode = GPIO_MODE_ALT;
	I2C_INIT.Alternate = 4;
	I2C_INIT.Pull = 1; // pull up
	I2C_INIT.Speed = 2; // high
	I2C_INIT.OutputType = 1; // open-drain

	I2C_INIT.Pin = 8;

	GPIO_Init(GPIOB, &I2C_INIT);

	I2C_INIT.Pin = 9;
	GPIO_Init(GPIOB, &I2C_INIT);


	*(volatile uint32_t *)(I2C1_BASE + I2C_CR2_OFFSET) |= (45U << 0); // set freq to 45MHZ
	
	*(volatile uint32_t *)(I2C1_BASE + I2C_CCR_OFFSET) |= (225U << 0); // 225 ticks (apb1)

	*(volatile uint32_t *)(I2C1_BASE + I2C_TRISE_OFFSET) |= (46U << 0); // wait 46 ticks for volatage to stablize

	*(volatile uint32_t *)(I2C1_BASE + I2C_CR1_OFFSET) |= (1U << 0); // enable peripheral

	return;
}


void I2C_Start(void){
	*(volatile uint32_t *)(I2C1_BASE + I2C_CR1_OFFSET) |= (1U << 8); 
	// controller mode : repeated start generation
	// target mode : start generation
	
	// wait for setting
	while ((*(volatile uint32_t *)(I2C1_BASE + I2C_SR1_OFFSET) & (1U << 0)) == 0){ // check for no start condition 
		continue;
	}
	return;
}

void I2C_Stop(void){
	*(volatile uint32_t *)(I2C1_BASE + I2C_CR1_OFFSET) |= (1U << 9);
	return;
}

void I2C_WriteAddress(uint8_t address){
	// overwrite register
	*(volatile uint32_t *)(I2C1_BASE + I2C_DR_OFFSET) = ((address << 1) << 0); // shift address by 1 so 0 bit is set to 0 (write mode)
	// also aht30 is 7bit
	
	while ((*(volatile uint32_t *)(I2C1_BASE + I2C_SR1_OFFSET) & (1 << 1)) == 0){
		continue; // check for addr to be set to 1 ( recieved address matched)
	}
	
	// dummy read to unfreeze the bus
	// gotta read sr1 and sr2 
	volatile uint32_t dummy_read = *(volatile uint32_t *)(I2C1_BASE + I2C_SR1_OFFSET);
	dummy_read =*(volatile uint32_t *)(I2C1_BASE + I2C_SR2_OFFSET);
	(void)dummy_read; 
}

void I2C_WriteData(uint8_t data){
	// write data into data register
	*(volatile uint32_t *)(I2C1_BASE + I2C_DR_OFFSET) = (data << 0);

	// wait for flag saying data is in the register
	while ((*(volatile uint32_t *)(I2C1_BASE + I2C_SR1_OFFSET) & (1U << 7)) == 0){
		continue;
	}
	return;
}

void I2C_ReadData(uint8_t *data_buffer, uint8_t size){
	*(volatile uint32_t *)(I2C1_BASE + I2C_CR1_OFFSET) |= (1U << 10); // enable acknowledge
	
	for (uint32_t i = 0; i < size; i++) {
		if (i == (size - 1)){
			*(volatile uint32_t *)(I2C1_BASE + I2C_CR1_OFFSET) &= ~(1U << 10); // disable acknowledge
			I2C_Stop(); // stop signal
			while ((*(volatile uint32_t *)(I2C1_BASE + I2C_SR1_OFFSET) & (1U << 6)) == 0){
				continue; // wait for data register to be empty?
			}
			*data_buffer = *(volatile uint32_t *)(I2C1_BASE + I2C_DR_OFFSET); // read last byte

		} else {
			while ((*(volatile uint32_t *)(I2C1_BASE + I2C_SR1_OFFSET) & (1U << 6)) == 0){
				continue;
			}
			*data_buffer = *(volatile uint32_t *)(I2C1_BASE + I2C_DR_OFFSET); // read byte
			data_buffer++; // increment to next byte position
			}
		}
}

