#include "rcc.h"
#include "gpio.h"
#include "systick.h"
#include "usart.h"
#include "i2c.h"

#include "board.h"
#include "aht30.h"
#include "SSD1306.h"

#include "app_sensor.h"
#include "app_display.h"
#include "app_log.h"

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
	Delay_ms(250);
	GPIO_WritePin(LED2_PORT, LED2_PIN, GPIO_PIN_RESET);
	Delay_ms(10);


	USART2_Init();
	USART2_WriteString("\nUSART INITIALIZED\r");

	I2C_Init();
	USART2_WriteString("\nI2C INITIALIZED\r");
	
	AHT30_Init();
	USART2_WriteString("\nSENSOR INITIALIZED\r");

	SSD1306_Init();
	USART2_WriteString("\nDISPLAY INITIALIZED\r\n");

	GPIO_WritePin(LED2_PORT, LED2_PIN, GPIO_PIN_SET);

	App_Sensor_Init();
	App_Display_Init();
	App_Log_Init();

	while (1) {
		App_Sensor_Run();

		App_Log_Run();

		App_Display_Run();

	}
	return 0;
}



