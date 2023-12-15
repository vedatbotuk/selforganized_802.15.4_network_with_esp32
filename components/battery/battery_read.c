/*
 * SPDX-FileCopyrightText: 2022-2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "soc/soc_caps.h"
#include "esp_log.h"
#include "esp_adc/adc_oneshot.h"

const static char *TAG_VOL = "VOLTAGE";

#define VOLTAGE_MAX 9000
#define VOLTAGE_MIN 7000
#define CHECK_ARG(VAL) do { if (!(VAL)) return ESP_ERR_INVALID_ARG; } while (0)

/*---------------------------------------------------------------
        ADC General Macros
---------------------------------------------------------------*/
//ADC1 Channels
#define EXAMPLE_ADC1_CHAN0          ADC_CHANNEL_0
#define EXAMPLE_ADC_ATTEN           ADC_ATTEN_DB_12

static adc_oneshot_unit_handle_t adc1_handle;
static int adc_raw[2][10];
void voltage_calculate_init();

int calc_battery_percentage(int adc)
{
    int battery_voltage = (float) adc * 519076 / 470000 / 3300 * VOLTAGE_MAX;
    ESP_LOGI(TAG_VOL, " ADC Raw: %d", adc);
    ESP_LOGI(TAG_VOL, "Battery voltage: %d mV", battery_voltage);
    int battery_percentage = 100 * (battery_voltage - VOLTAGE_MIN) / (VOLTAGE_MAX - VOLTAGE_MIN);

    if (battery_percentage < 0)
        battery_percentage = 0;
    
    return battery_percentage;
}

esp_err_t get_battery_level(int *battery_level, int *voltage_cal)
{
    CHECK_ARG(battery_level || voltage_cal);
    ESP_ERROR_CHECK(adc_oneshot_read(adc1_handle, EXAMPLE_ADC1_CHAN0, &adc_raw[0][0]));
    ESP_LOGI(TAG_VOL, "ADC%d Channel[%d] Raw Data: %d", ADC_UNIT_1 + 1, EXAMPLE_ADC1_CHAN0, adc_raw[0][0]); 

    *voltage_cal = adc_raw[0][0] / 10;
    *battery_level = calc_battery_percentage(adc_raw[0][0]);
    
    return ESP_OK;
}

esp_err_t voltage_calculate_init(void)
{
    //-------------ADC1 Init---------------//
    adc_oneshot_unit_init_cfg_t init_config1 = {
        .unit_id = ADC_UNIT_1,
    };
    ESP_ERROR_CHECK(adc_oneshot_new_unit(&init_config1, &adc1_handle));

    //-------------ADC1 Config---------------//
    adc_oneshot_chan_cfg_t config = {
        .bitwidth = ADC_BITWIDTH_DEFAULT,
        .atten = EXAMPLE_ADC_ATTEN,
    };
    
    esp_err_t err;
    err = ESP_ERROR_CHECK(adc_oneshot_config_channel(adc1_handle, EXAMPLE_ADC1_CHAN0, &config));
    if (err != ESP_OK) {
        return err;
    }

    return ESP_OK;
}
