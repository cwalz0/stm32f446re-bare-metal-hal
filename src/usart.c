#include "usart.h"
#include "gpio.h"


#include <stdint.h>

void USART2_Init(){
	GPIO_InitTypeDef USART2_INIT;

	USART2_INIT.Mode = GPIO_MODE_ALT;
	USART2_INIT.Pull = 0;
	USART2_INIT.Speed = 3;
	USART2_INIT.Alternate = 7;

	USART2_INIT.Pin = 2; // Port A Pin 2 (PA 2) : TX

	GPIO_Init(GPIOA, &USART2_INIT);

	USART2_INIT.Pin = 3; // PA3 : RX
	GPIO_Init(GPIOA, &USART2_INIT);


	return;
}
