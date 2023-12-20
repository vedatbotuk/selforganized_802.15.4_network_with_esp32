#ifndef LIGHT_SLEEP_H
#define LIGHT_SLEEP_H

#include "esp_zigbee_core.h"
#include "esp_err.h"
#include "esp_pm.h"
#include "esp_private/esp_clk.h"

#ifdef __cplusplus
extern "C"
{
#endif

    esp_err_t esp_zb_power_save_init(int cpu_freq);
    void sleep_enable();
    void sleep_configure();

#ifdef __cplusplus
}
#endif

#endif /* LIGHT_SLEEP_H */
