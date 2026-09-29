#include "app_log.h"
#include "app_sensor.h"
#include "usart.h"
#include "systick.h"
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/types.h>

typedef enum {
	LOG_STATE_IDLE,
	LOG_STATE_TRANSMIT
} LogState_t;

static LogState_t current_log_state = LOG_STATE_IDLE;
static uint32_t log_timer_start = 0;

void App_Log_Init(void){
	current_log_state = LOG_STATE_IDLE;
	log_timer_start = SysTick_GetMillis();
}

void App_Log_Run(void){
	switch (current_log_state){

		case LOG_STATE_IDLE:
			// once per sec
			if ((SysTick_GetMillis() - log_timer_start) >= 1000) {
				log_timer_start = SysTick_GetMillis();
				current_log_state = LOG_STATE_TRANSMIT;
			}
			break;

		case LOG_STATE_TRANSMIT:
			{
			char log_buffer[64];
			int temp_whole = (int)app_temperature_f;
			int temp_dec = (int)((app_temperature_f - temp_whole) * 100 );

			int humidity_whole = (int)app_humidity;
			int humidity_dec = (int)((app_humidity - humidity_whole) * 100);

			sprintf(log_buffer, "[LOG] Temp: %d.%d02 F | Hum: %d.%02d%%\r\n", temp_whole, temp_dec, humidity_whole, humidity_dec);

			USART2_WriteString(log_buffer);	
			
			current_log_state = LOG_STATE_IDLE;
			}
		break;
	}
}
