#include "gpio.h"
#include <stdint.h>

void GPIO_Init(uint32_t port_base, GPIO_InitTypeDef *init_struct){
	uint32_t pin = init_struct->Pin;

	volatile uint32_t *moder = (volatile uint32_t *)(port_base + GPIO_MODER_OFFSET);
	volatile uint32_t *otyper = (volatile uint32_t *)(port_base + GPIO_OTYPER_OFFSET);
	volatile uint32_t *ospeedr = (volatile uint32_t *)(port_base + GPIO_OSPEEDR_OFFSET);
	volatile uint32_t *pupdr = (volatile uint32_t *)(port_base + GPIO_PUPDR_OFFSET);

	//volatile uint32_t *idr = (volatile uint32_t *)(port_base + GPIO_IDR_OFFSET);
	//volatile uint32_t *odr = (volatile uint32_t *)(port_base + GPIO_ODR_OFFSET);
	//volatile uint32_t *bssr = (volatile uint32_t *)(port_base + GPIO_BSRR_OFFSET);
	//volatile uint32_t *lckr = (volatile uint32_t *)(port_base + GPIO_LCKR_OFFSET);
	volatile uint32_t *afrl = (volatile uint32_t *)(port_base + GPIO_AFRL_OFFSET);
	volatile uint32_t *afrh = (volatile uint32_t *)(port_base + GPIO_AFRH_OFFSET);
	
	if (init_struct->Mode == GPIO_MODE_ALT){
		if (init_struct->Pin < 8) {
			*afrl &= ~(15U << pin * 4U);
			*afrl |= (init_struct->Alternate << (pin * 4));
		} else if (init_struct->Pin >= 8) {
			*afrh &= ~(15U << (pin - 8) * 4U);
			*afrh |= (init_struct->Alternate << (pin - 8) * 4);
		}
	}

	*moder &= ~(3U << (pin * 2U)); // clear 2 pins
	*moder |= (init_struct->Mode << (pin * 2)); // 0, 1, 2, 3
	
	*otyper &= ~(1U << pin); // clear 1 pins
	*otyper |= (init_struct->OutputType << pin); // 0, 1
	
	*ospeedr &= ~(3U << (pin * 2));
	*ospeedr |= (init_struct->Speed << (pin * 2)); // 0, 1, 2, 3

	*pupdr &= ~(3U << (pin * 2));
	*pupdr |= (init_struct->Pull << (pin * 2)); // 0, 1, 2
}

void GPIO_WritePin(uint32_t port_base, uint8_t pin, GPIO_PinState state) {
	volatile uint32_t *bssr = (volatile uint32_t *)(port_base + GPIO_BSRR_OFFSET);

	if (state == GPIO_PIN_SET) {
		*bssr = (1U << pin); // lower 16 bits to SET
	} else {
		*bssr = (1U << (pin + 16U)); // Upper 16 bits to SET
	}
}


