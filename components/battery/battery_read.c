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

#define VOLTAGE_MAX 8200
#define VOLTAGE_MIN 7000

/*---------------------------------------------------------------
        ADC General Macros
---------------------------------------------------------------*/
//ADC1 Channels
#if CONFIG_IDF_TARGET_ESP32C6
#define EXAMPLE_ADC1_CHAN1          ADC_CHANNEL_3
#elif CONFIG_IDF_TARGET_ESP32H2
#define EXAMPLE_ADC1_CHAN1          ADC_CHANNEL_0
#endif

#define EXAMPLE_ADC_ATTEN           ADC_ATTEN_DB_12

adc_oneshot_unit_handle_t adc1_handle;

static int adc_raw[2][10];
void voltage_calculate_init();

int calc_battery_percentage(int adc)
{
    int battery_voltage = (float) adc * 519076 / 470000 / 3300 * VOLTAGE_MAX;
    int battery_percentage = 100 * (battery_voltage - VOLTAGE_MIN) / (VOLTAGE_MAX - VOLTAGE_MIN);

//    if (battery_percentage < 0)
//        battery_percentage = 0;
//    if (battery_percentage > 100)
//        battery_percentage = 100;

    return battery_percentage;
}

int get_battery_level()
{
    int battery_level = 0;

    ESP_ERROR_CHECK(adc_oneshot_read(adc1_handle, EXAMPLE_ADC1_CHAN1, &adc_raw[0][0]));
    ESP_LOGI(TAG_VOL, "ADC%d Channel[%d] Raw Data: %d", ADC_UNIT_1 + 1, EXAMPLE_ADC1_CHAN1, adc_raw[0][0]);
    battery_level = calc_battery_percentage(adc_raw[0][0]);

    return battery_level;
}

void voltage_calculate_init(){
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
    ESP_ERROR_CHECK(adc_oneshot_config_channel(adc1_handle, EXAMPLE_ADC1_CHAN1, &config));
}
