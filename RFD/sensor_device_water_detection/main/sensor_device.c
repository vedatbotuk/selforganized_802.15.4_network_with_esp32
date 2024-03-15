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

#include "sensor_device.h"
#include "nvs_flash.h"
#include "esp_check.h"
#include "esp_log.h"
#if (SENSOR_MAP >> 6 & 1) == 1
#include "ota.h"
#endif
#include "update_cluster.h"
#include "create_cluster.h"
#if (SENSOR_MAP >> 2 & 1) == 1
#include "light_sleep.h"
#endif
#include "waterleak.h"
#include "signal_handler.h"

#if !defined ZB_ED_ROLE
#error Define ZB_ED_ROLE in idf.py menuconfig to compile RFD (End Device) source code.
#endif

static char firmware_version[16] = {7, 'v', 'e', 'r', '0', '.', '1', '3'};
static const char *TAG = "SENSOR_DEVICE";

/********************* Define functions **************************/
void esp_zb_app_signal_handler(esp_zb_app_signal_t *signal_struct)
{
    create_signal_handler(*signal_struct);
}

#if (SENSOR_MAP >> 6 & 1) == 1
static esp_err_t zb_action_handler(esp_zb_core_action_callback_id_t callback_id, const void *message)
{
    esp_err_t ret = ESP_OK;
    switch (callback_id)
    {
    case ESP_ZB_CORE_OTA_UPGRADE_VALUE_CB_ID:
        ret = zb_ota_upgrade_status_handler(*(esp_zb_zcl_ota_upgrade_value_message_t *)message);
        break;
    default:
        ESP_LOGW(TAG, "Receive Zigbee action(0x%x) callback", callback_id);
        break;
    }
    return ret;
}
#endif

static void esp_zb_task(void *pvParameters)
{
    /* initialize Zigbee stack */
    esp_zb_cfg_t zb_nwk_cfg = ESP_ZB_ZED_CONFIG();
    /* The order in the following 3 lines must not be changed. */
#if (SENSOR_MAP >> 2 & 1) == 1
    sleep_enable();
#endif
    esp_zb_init(&zb_nwk_cfg);
#if (SENSOR_MAP >> 2 & 1) == 1
    sleep_configure();
#endif
    esp_zb_set_tx_power(TX_POWER);

    /* create cluster lists for this endpoint */
    esp_zb_cluster_list_t *esp_zb_cluster_list = esp_zb_zcl_cluster_list_create();

    create_basic_cluster(esp_zb_cluster_list, firmware_version);
    create_identify_cluster(esp_zb_cluster_list);
    create_waterleak_cluster(esp_zb_cluster_list);
    create_battery_cluster(esp_zb_cluster_list);
#if (SENSOR_MAP >> 6 & 1) == 1
    create_ota_cluster(esp_zb_cluster_list);
#endif

    esp_zb_ep_list_t *esp_zb_ep_list = esp_zb_ep_list_create();
    esp_zb_ep_list_add_ep(esp_zb_ep_list, esp_zb_cluster_list, DEVICE_ENDPOINT, ESP_ZB_AF_HA_PROFILE_ID, ESP_ZB_HA_TEMPERATURE_SENSOR_DEVICE_ID);

    esp_zb_device_register(esp_zb_ep_list);
#if (SENSOR_MAP >> 6 & 1) == 1
    esp_zb_core_action_handler_register(zb_action_handler);
#endif
    esp_zb_set_primary_network_channel_set(ESP_ZB_PRIMARY_CHANNEL_MASK);
    ESP_ERROR_CHECK(esp_zb_start(false));
    esp_zb_main_loop_iteration();
}

void app_main(void)
{
    ESP_LOGI(TAG, "--- Application Start ---");
    esp_zb_platform_config_t config = {
        .radio_config = ESP_ZB_DEFAULT_RADIO_CONFIG(),
        .host_config = ESP_ZB_DEFAULT_HOST_CONFIG(),
    };
  
    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_zb_platform_config(&config));
#if (SENSOR_MAP >> 2 & 1) == 1
    ESP_ERROR_CHECK(esp_zb_power_save_init(CONFIG_ESP_DEFAULT_CPU_FREQ_MHZ));
#endif
    xTaskCreate(esp_zb_task, "Zigbee_main", 4096, NULL, 5, NULL);
}