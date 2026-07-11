#ifndef USART_H
#define USART_H

#define USART2_BASE 0x40004400U

#define USART_SR_OFFSET 0x00U
#define USART_DR_OFFSET 0x04U
#define USART_BRR_OFFSET 0x08U
#define USART_CR1_OFFSET 0x0CU
#define USART_CR2_OFFSET 0x10U
#define USART_CR3_OFFSET 0x14U
#define USART_GTPR_OFFSET 0x18U

void USART2_Init(void);
void USART2_WriteChar(int ch);



#endif
