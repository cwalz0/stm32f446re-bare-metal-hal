#ifndef SPI_H
#define SPI_H

#define SPI2_BASE 0x40003800U
#define SPI_CR1_OFFSET 0x00U
#define SPI_CR2_OFFSET 0x04U
#define SPI_SR_OFFSET 0x08U
#define SPI_DR_OFFSET 0x0CU
#define SPI_CRCPR_OFFSET 0x10U
#define SPI_RXCRCR_OFFSET 0x14U
#define SPI_TXCRCR_OFFSET 0x18U
#define SPI_I2SCFGR_OFFSET 0x1CU
#define SPI_I2SPR_OFFSET 0x20U

#include <stdint.h>

void SPI2_Init(void);
void SPI2_WriteByte(uint8_t data);
#endif

