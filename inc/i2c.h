#ifndef I2C_H
#define I2C_H
#include <stdint.h>
#define I2C1_BASE 0x40005400U
#define I2C_CR1_OFFSET 0x00U
#define I2C_CR2_OFFSET 0x04U
#define I2C_OAR1_OFFSET 0X08U
#define I2C_OAR2_OFFSET 0x0CU
#define I2C_DR_OFFSET 0x10U
#define I2C_SR1_OFFSET 0x14U
#define I2C_SR2_OFFSET 0x18U
#define I2C_CCR_OFFSET  0x1CU
#define I2C_TRISE_OFFSET  0x20U
#define I2C_FLTR_OFFSET  0x24U


void I2C_Init(void);
void I2C_Start(void);
void I2C_Stop(void);
void I2C_WriteAddress(uint8_t address);
void I2C_WriteData(uint8_t data);
void I2C_ReadData(uint8_t *buffer, uint8_t size);

#endif
