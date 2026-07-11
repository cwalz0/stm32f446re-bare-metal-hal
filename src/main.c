#include "rcc.h"
#include "gpio.h"
#include "systick.h"
#include "usart.h"

#include "board.h"
#include <stdint.h>

int main(void) {
	SystemClockConfig();

	RCC_EnableClock(AHB1ENR_OFFSET, 0);

	SysTick_Init();

	GPIO_InitTypeDef led_init = {0};
	led_init.Pin = LED2_PIN;
	led_init.Mode = GPIO_MODE_OUTPUT;
	

	GPIO_Init(LED2_PORT, &led_init);

	GPIO_WritePin(LED2_PORT, LED2_PIN, GPIO_PIN_SET);
	Delay_ms(500);
	GPIO_WritePin(LED2_PORT, LED2_PIN, GPIO_PIN_RESET);
	Delay_ms(500);


	USART2_Init();
	USART2_WriteString("\nUSART INITIALIZED\r\n");
	GPIO_WritePin(LED2_PORT, LED2_PIN, GPIO_PIN_SET);


	
	while (1) {
		GPIO_WritePin(LED2_PORT, LED2_PIN, GPIO_PIN_SET);
		USART2_WriteString("LED ON\r\n");
		Delay_ms(1000);

		GPIO_WritePin(LED2_PORT, LED2_PIN, GPIO_PIN_RESET);
		USART2_WriteString("LED OFF\r\n");
		Delay_ms(1000);
	}
	return 0;
}
