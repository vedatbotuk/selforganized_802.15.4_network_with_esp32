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
    esp_zb_sleep_set_threshold(20);
}
