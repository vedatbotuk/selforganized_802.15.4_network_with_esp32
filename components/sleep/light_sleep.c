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
#include "light_sleep.h"
#include "esp_pm.h"
#include "esp_private/esp_clk.h"

esp_err_t esp_zb_power_save_init(int cpu_freq)
{
    esp_err_t rc = ESP_OK;
    int cur_cpu_freq_mhz = cpu_freq;
    esp_pm_config_t pm_config = {
        .max_freq_mhz = cur_cpu_freq_mhz,
        .min_freq_mhz = cur_cpu_freq_mhz,
        .light_sleep_enable = true};
    rc = esp_pm_configure(&pm_config);
    return rc;
}

void sleep_enable()
{
    esp_zb_sleep_enable(true);
}

void sleep_configure()
{
    esp_zb_sleep_set_threshold(2000);
}
