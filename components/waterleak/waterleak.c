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

#include "waterleak.h"
#include "update_cluster.h"
#include "driver/gpio.h"
#include "esp_log.h"

#define INPUT_PIN GPIO_NUM_22
static const char *TAG = "WATERLEAK_CHECK";

bool water_detected = false;
uint8_t button_cnt = 0;

esp_err_t check_waterleak(void)
{
    if (gpio_get_level(INPUT_PIN) == 0 && water_detected == false)
    {
        ESP_LOGI(TAG, "Water detected");
        zb_update_waterleak(10, 1);
        zb_report_waterleak(10, 1);
        water_detected = true;
        button_cnt = 0;
    } else {
        if (water_detected == true)
        {
            ESP_LOGI(TAG, "Water alarm released");
            zb_update_waterleak(10, 0);
            zb_report_waterleak(10, 0);
            water_detected = false;
            button_cnt = 0;
        }
    }

    /* Every 5 Minutes*/
    if (button_cnt >= 30)
    {
        if (water_detected == true)
        {
            water_detected = false;
        }
        else
        {
            water_detected = true;
        }
        button_cnt = 0;
    }

    button_cnt++;
    return ESP_OK;
}

esp_err_t button_init(void)
{
    gpio_set_direction(INPUT_PIN, GPIO_MODE_INPUT);
    gpio_set_pull_mode(INPUT_PIN, GPIO_PULLUP_ONLY);
    return ESP_OK;
}