#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>

#define GPIOA	0x40020000U
#define GPIOB	0x40020400U
#define GPIOC	0x40020800U
#define GPIOD	0x40020C00U
#define GPIOE	0x40021000U
#define GPIOF	0x40021400U
#define GPIOG	0x40021800U
#define GPIOH	0x40021C00U

#define GPIO_MODER_OFFSET	0x00U
#define GPIO_OTYPER_OFFSET	0x04U
#define GPIO_OSPEEDR_OFFSET	0x08U
#define GPIO_PUPDR_OFFSET	0x0CU
#define GPIO_IDR_OFFSET		0x10U
#define GPIO_ODR_OFFSET		0x14U
#define GPIO_BSRR_OFFSET	0x18U
#define GPIO_LCKR_OFFSET	0x1CU
#define GPIO_AFRL_OFFSET	0x20U
#define GPIO_AFRH_OFFSET	0x24U

#define GPIO_MODE_INPUT  0x00U 
#define GPIO_MODE_OUTPUT 0x01U 
#define GPIO_MODE_ALT    0x02U 
#define GPIO_MODE_ANALOG 0x03U

typedef enum{
	GPIO_PIN_RESET = 0U,
	GPIO_PIN_SET = 1U
} GPIO_PinState;

typedef struct{
	uint8_t Pin; 	// Which pin
	uint8_t Mode;	// input   / output  / alt         / analog
	uint8_t Pull;	// no pull / pull up / pull down
	uint8_t Speed;	// low     / medium  /fast         / high
	uint8_t OutputType; // push pull / open-drain
	uint8_t Alternate; 
}GPIO_InitTypeDef;

void GPIO_Init(uint32_t port_base, GPIO_InitTypeDef *init_struct);

void GPIO_WritePin(uint32_t port_base, uint8_t pin, GPIO_PinState state);


#endif

