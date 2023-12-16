// zb_temp_sensor.h

#ifndef ZB_TEMP_SENSOR_H
#define ZB_TEMP_SENSOR_H

#include "esp_zigbee_core.h"
#include "esp_log.h"
#include <stdint.h>

void zb_update_temp(int temperature, uint8_t endpoint);
void zb_update_hum(int humidity, uint8_t endpoint);
void zb_update_battery_level(int level, int voltage, uint8_t endpoint);

#endif // ZB_TEMP_SENSOR_H

