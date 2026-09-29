#ifndef APP_SENSOR_H
#define APP_SENSOR_H

#include <stdint.h>

extern float app_temperature_f;
extern float app_humidity;
void App_Sensor_Init(void);
void App_Sensor_Run(void);

#endif

