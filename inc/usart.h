#ifndef USART_H
#define USART_H

#include <stdint.h>
#define USART2_BASE 0x40004400U

#define USART_SR_OFFSET 0x00U
#define USART_DR_OFFSET 0x04U
#define USART_BRR_OFFSET 0x08U
#define USART_CR1_OFFSET 0x0CU
#define USART_CR2_OFFSET 0x10U
#define USART_CR3_OFFSET 0x14U
#define USART_GTPR_OFFSET 0x18U

void USART2_Init(void);
int USART2_ReadChar(void);
void USART2_WriteChar(int ch);
void USART2_WriteInt(int value);
void USART2_WriteFloat(float value);
void USART2_WriteString(char *str);
void USART2_WriteHex(uint8_t value);



#endif
