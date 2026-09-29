#include "app_display.h"
#include "app_sensor.h"
#include "SSD1306.h"
#include "splash_bitmap.h"
#include "systick.h"
#include <stdint.h>
#include <stdio.h>

#define SPLASH_LENGTH 3000
#define REFRESH_LENGTH 1000

typedef enum {
	DISPLAY_STATE_SPLASH_INIT,
	DISPLAY_STATE_SPLASH_WAIT,
	DISPLAY_STATE_DASHBOARD
} DisplayState_t;


DisplayState_t current_display_state = DISPLAY_STATE_SPLASH_INIT;
static uint32_t display_timer_start = 0;

void App_Display_Init(void){
	current_display_state = DISPLAY_STATE_SPLASH_INIT;
}

void App_Display_Run(){
	switch (current_display_state) {
		case DISPLAY_STATE_SPLASH_INIT:
			SSD1306_ClearBuffer();
			SSD1306_DrawBitmap(splash_bitmap);
			SSD1306_UpdateDisplay();
			display_timer_start = SysTick_GetMillis();
			current_display_state = DISPLAY_STATE_SPLASH_WAIT;

		case DISPLAY_STATE_SPLASH_WAIT:
			if ((SysTick_GetMillis() - display_timer_start) >= SPLASH_LENGTH){
				SSD1306_ClearBuffer();
				display_timer_start = SysTick_GetMillis();
				current_display_state = DISPLAY_STATE_DASHBOARD;
			}
		break;

		case DISPLAY_STATE_DASHBOARD:
		if ((SysTick_GetMillis() - display_timer_start) >= REFRESH_LENGTH) {
				display_timer_start = SysTick_GetMillis();

				SSD1306_ClearBuffer();

				int temp_whole = (int)app_temperature_f;
				int temp_decimal = (int)((app_temperature_f - temp_whole) * 100);

				int humidity_whole = (int)app_humidity;
				int humidity_decimal = (int)((app_humidity - humidity_whole) * 100);

				char temp_str[20];
				char humidity_str[20];

				sprintf(temp_str, "TEMP: %d.%02d F", temp_whole, temp_decimal);
				sprintf(humidity_str, " HUM: %d.%02d %%", humidity_whole, humidity_decimal);

				SSD1306_DrawString(temp_str, 0, 0, 1);
				SSD1306_DrawString(humidity_str, 0, 16, 1);

				SSD1306_UpdateDisplay();
			}
		break;
	}
}
