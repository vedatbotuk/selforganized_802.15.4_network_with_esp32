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

bool _water_detected = false;
uint8_t _button_cnt = 0;
bool _btn_init_status;

esp_err_t check_waterleak(void)
{
    if (_btn_init_status == false)
    {
        if (button_init() == ESP_OK)
        {
            _btn_init_status = true;
        }
    }

    if (gpio_get_level(INPUT_PIN) == 0 && _water_detected == false)
    {
        ESP_LOGI(TAG, "Water detected");
        zb_update_waterleak(10, 1);
        zb_report_waterleak(10, 1);
        _water_detected = true;
        _button_cnt = 0;
        return ESP_OK;
    }

    if (gpio_get_level(INPUT_PIN) == 1 && _water_detected == true)
    {

        ESP_LOGI(TAG, "Water alarm released");
        zb_update_waterleak(10, 0);
        zb_report_waterleak(10, 0);
        _water_detected = false;
        _button_cnt = 0;
        return ESP_OK;
    }

    /* Every 5 Minutes*/
    if (_button_cnt >= 30)
    {
        if (_water_detected == true)
        {
            _water_detected = false;
        }
        else
        {
            _water_detected = true;
        }
        _button_cnt = 0;
    }

    _button_cnt++;
    return ESP_OK;
}

esp_err_t button_init(void)
{
    gpio_set_direction(INPUT_PIN, GPIO_MODE_INPUT);
    gpio_set_pull_mode(INPUT_PIN, GPIO_PULLUP_ONLY);
    return ESP_OK;
}