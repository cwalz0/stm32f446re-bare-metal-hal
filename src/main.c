#include "rcc.h"
#include "gpio.h"
#include "systick.h"

#include "board.h"
#include <stdint.h>

int main(void) {
	SystemClockConfig();

	RCC_EnableClock(AHB1ENR_OFFSET, 0);

	SysTick_Init();

	GPIO_InitTypeDef led_init;
	led_init.Pin = LED2_PIN;
	led_init.Mode = GPIO_MODE_OUTPUT;
	led_init.Speed = 0;
	led_init.Pull = 0;
	led_init.OutputType = 0;
	

	GPIO_Init(LED2_PORT, &led_init);

	
	while (1) {
		GPIO_WritePin(LED2_PORT, LED2_PIN, GPIO_PIN_SET);

		Delay_ms(250);

		GPIO_WritePin(LED2_PORT, LED2_PIN, GPIO_PIN_RESET);

		Delay_ms(250);
	}
	return 0;
}
