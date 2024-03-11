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
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "soc/soc_caps.h"
#include "esp_log.h"
#include "esp_adc/adc_oneshot.h"

const static char *TAG_VOL = "VOLTAGE";

#define VOLTAGE_MAX 3000
#define VOLTAGE_MIN 1500
#define CHECK_ARG(VAL)                  \
    do                                  \
    {                                   \
        if (!(VAL))                     \
            return ESP_ERR_INVALID_ARG; \
    } while (0)

/*---------------------------------------------------------------
        ADC General Macros
---------------------------------------------------------------*/
// ADC1 Channels
#define EXAMPLE_ADC1_CHAN0 ADC_CHANNEL_0
#define EXAMPLE_ADC_ATTEN ADC_ATTEN_DB_12

static adc_oneshot_unit_handle_t adc1_handle;
static int adc_raw[2][10];
esp_err_t voltage_calculate_init(void);

uint8_t calc_battery_percentage(int adc)
{
    /* Calculate for 2x3.7V Batteries*/
//    int battery_voltage = (float)adc * 519076 / 470000 / 3300 * VOLTAGE_MAX;
//    int battery_percentage = 100 * (battery_voltage - VOLTAGE_MIN) / (VOLTAGE_MAX - VOLTAGE_MIN);

    /*For 3V no calculating is necassary*/
    int battery_percentage = 100 * ((float)adc - VOLTAGE_MIN) / (VOLTAGE_MAX - VOLTAGE_MIN);

//    ESP_LOGI(TAG_VOL, " ADC Raw: %d", adc);
//    ESP_LOGI(TAG_VOL, "Battery percentage: %d %%", battery_percentage);

    if (battery_percentage < 0)
        battery_percentage = 0;

    return battery_percentage;
}

esp_err_t get_battery_level(uint8_t *battery_level, uint8_t *voltage_cal)
{
    CHECK_ARG(battery_level || voltage_cal);
    ESP_ERROR_CHECK(adc_oneshot_read(adc1_handle, EXAMPLE_ADC1_CHAN0, &adc_raw[0][0]));
    ESP_LOGI(TAG_VOL, "ADC%d Channel[%d] Raw Data: %d", ADC_UNIT_1 + 1, EXAMPLE_ADC1_CHAN0, adc_raw[0][0]);

    *voltage_cal = (uint8_t)(adc_raw[0][0] / 10);
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

    // ESP_ERROR_CHECK(adc_oneshot_config_channel(adc1_handle, EXAMPLE_ADC1_CHAN0, &config));

    esp_err_t res = adc_oneshot_config_channel(adc1_handle, EXAMPLE_ADC1_CHAN0, &config);
    if (res != ESP_OK)
        return res;

    // TODO
    // esp_err_t err;
    // err = ESP_ERROR_CHECK(adc_oneshot_config_channel(adc1_handle, EXAMPLE_ADC1_CHAN0, &config));
    // if (err != ESP_OK) {
    // return err;
    // }

    return ESP_OK;
}

esp_err_t voltage_calculate_deinit(void)
{
    //Tear Down
    esp_err_t res = adc_oneshot_del_unit(adc1_handle);
    if (res != ESP_OK)
        return res;

    return ESP_OK;
}
