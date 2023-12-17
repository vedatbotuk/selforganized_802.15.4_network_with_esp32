/*
 * SPDX-FileCopyrightText: 2022-2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef VOLTAGE_CALCULATE_H
#define VOLTAGE_CALCULATE_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "soc/soc_caps.h"
#include "esp_log.h"
#include "esp_adc/adc_oneshot.h"

#ifdef __cplusplus
extern "C" {
#endif

extern const static char *TAG_VOL;
extern const int VOLTAGE_MAX;
extern const int VOLTAGE_MIN;

esp_err_t voltage_calculate_init(void);
int calc_battery_percentage(int adc);
esp_err_t get_battery_level(int *battery_level, int *voltage_cal);

#ifdef __cplusplus
}
#endif

#endif /* VOLTAGE_CALCULATE_H */
