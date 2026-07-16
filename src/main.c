#include "rcc.h"
#include "gpio.h"
#include "systick.h"
#include "usart.h"

#include "board.h"
#include <stdint.h>

int main(void) {

	*(volatile uint32_t *)(0xE000ED00 + 0x88) |= (0xF << 20);

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
		// wait for user input from terminal
		int input_char = USART2_ReadChar();

		USART2_WriteChar(input_char); // output user command
		USART2_WriteString("\r\n"); // new line

		if (input_char == '1') {
			GPIO_WritePin(LED2_PORT, LED2_PIN, GPIO_PIN_SET);
			USART2_WriteString("COMMAND: LED ON\r\n");
		} else if (input_char == '0') {
			GPIO_WritePin(LED2_PORT, LED2_PIN, GPIO_PIN_RESET);
			USART2_WriteString("COMMAND: LED OFF\r\n");
		}
	}
	return 0;
}
