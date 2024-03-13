/* 
 * selforganized_802.15.4_network_with_esp32
 * Copyright (c) 2024 Vedat Botuk.
 * 
 * This program is free software: you can redistribute it and/or modify  
 * it under the terms of the GNU General Public License as published by  
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef ZB_TEMP_SENSOR_H
#define ZB_TEMP_SENSOR_H

#include "esp_zigbee_core.h"
#include "esp_log.h"
#include <stdint.h>
#include "zcl/esp_zigbee_zcl_power_config.h"

void zb_update_temp(int temperature, uint8_t endpoint);
void zb_report_temp(uint8_t endpoint);
void zb_update_hum(int humidity, uint8_t endpoint);
void zb_report_hum(uint8_t endpoint);
void zb_update_battery_level(int level, int voltage, uint8_t endpoint);
void zb_update_waterleak(uint8_t endpoint, uint16_t leak);
void zb_report_waterleak(uint8_t endpoint, uint16_t leak);

#endif // ZB_TEMP_SENSOR_H

