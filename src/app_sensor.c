#include "app_sensor.h"
#include "aht30.h"
#include "systick.h"
#include <stdint.h>

typedef enum {
	SENSOR_STATE_IDLE,
	SENSOR_STATE_WAITING_FOR_DATA
} SensorState_t;


static SensorState_t current_state = SENSOR_STATE_IDLE;
static uint32_t timer_start = 0;
static uint8_t raw_data[6] = {0};

float app_temperature_f = 0.0f;
float app_humidity = 0.0f;

void App_Sensor_Init(void){
	current_state = SENSOR_STATE_IDLE;
}

void App_Sensor_Run(void){
	switch (current_state){

		case SENSOR_STATE_IDLE:
			AHT30_Trigger_Measurement();
			timer_start = SysTick_GetMillis();
			current_state = SENSOR_STATE_WAITING_FOR_DATA;
			break;

		case SENSOR_STATE_WAITING_FOR_DATA:
			if ((SysTick_GetMillis() - timer_start) >= 80 ) {
				AHT30_Read_Measurement(raw_data);
				app_temperature_f = AHT30_GetTempF(raw_data);
				app_humidity = AHT30_GetHumidity(raw_data);

				current_state = SENSOR_STATE_IDLE;
			}
		break;
	}
}
