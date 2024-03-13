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

#ifndef VOLTAGE_CALCULATE_H
#define VOLTAGE_CALCULATE_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "soc/soc_caps.h"
#include "esp_log.h"
#include "esp_adc/adc_oneshot.h"

#ifdef __cplusplus
extern "C"
{
#endif

    // extern const static char *TAG_VOL;
    extern const int VOLTAGE_MAX;
    extern const int VOLTAGE_MIN;

    esp_err_t voltage_calculate_init(void);
    esp_err_t voltage_calculate_deinit(void);
    int calc_battery_percentage(int adc);
    esp_err_t get_battery_level();

#ifdef __cplusplus
}
#endif

#endif /* VOLTAGE_CALCULATE_H */
