// zb_temp_sensor.h

#ifndef ZB_TEMP_SENSOR_H
#define ZB_TEMP_SENSOR_H

#include "esp_zigbee.h"
#include "esp_log.h"

#define SENSOR_DEVICE_ENDPOINT 1 // Update with your actual endpoint value

void zb_update_temp(int temperature);
void zb_update_hum(int humidity);
void zb_update_battery_level(int level, int voltage);

#endif // ZB_TEMP_SENSOR_H

