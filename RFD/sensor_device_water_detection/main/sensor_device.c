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
#include "ota.c"
#include "update_cluster.h"
#include "create_cluster.h"
#include "light_sleep.h"
#include "signal_handler.h"
#include "driver/gpio.h"
#include "driver/gpio.h"

#if !defined ZB_ED_ROLE
#error Define ZB_ED_ROLE in idf.py menuconfig to compile RFD (End Device) source code.
#endif

#if CONFIG_IDF_TARGET_ESP32C6
#define CONFIG_EXAMPLE_DATA_GPIO GPIO_NUM_2
#elif CONFIG_IDF_TARGET_ESP32H2
#define CONFIG_EXAMPLE_DATA_GPIO GPIO_NUM_0
#endif

#define SENSOR_TYPE DHT_TYPE_AM2301

static char firmware_version[16] = {7, 'v', 'e', 'r', '0', '.', '1', '3'};
static const char *TAG = "SENSOR_DEVICE";
bool connected = false;
bool water_detected = false;

#define INPUT_PIN GPIO_NUM_22

/********************* Define functions **************************/

static void check_water_leak()
{
    bool water_detected = false;
    /* because of cnt will be resend command again.*/
    uint8_t cnt = 0;
    
    while(1) {
        if (connection_status() == true) {
            if (gpio_get_level(INPUT_PIN) == 0 && water_detected == false) {
                ESP_LOGI(TAG, "Water detected");
                zb_update_waterleak(SENSOR_DEVICE_ENDPOINT, 1);
                zb_report_waterleak(SENSOR_DEVICE_ENDPOINT, 1);
                water_detected = true;
                cnt = 0;
            }
            
            if (gpio_get_level(INPUT_PIN) == 1 && water_detected == true) {
                ESP_LOGI(TAG, "Water alarm released");
                zb_update_waterleak(SENSOR_DEVICE_ENDPOINT, 0);
                zb_report_waterleak(SENSOR_DEVICE_ENDPOINT, 0);
                water_detected = false;
                cnt = 0;
            }
            
            /* Every 5 Minutes*/
            if (cnt >= 30) {
                if (water_detected == true) {water_detected = false;} else {water_detected = true;}
                cnt = 0;
                }
            cnt++;
            
        } else {
            ESP_LOGI(TAG, "Device is not connected!");
        }
        vTaskDelay(pdMS_TO_TICKS(10000));
    }
}


void esp_zb_app_signal_handler(esp_zb_app_signal_t *signal_struct){
    create_signal_handler_normal(*signal_struct);
}

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

static void esp_zb_task(void *pvParameters)
{
    /* initialize Zigbee stack */
    esp_zb_cfg_t zb_nwk_cfg = ESP_ZB_ZED_CONFIG();
    /* The order in the following 3 lines must not be changed. */
    sleep_enable();
    esp_zb_init(&zb_nwk_cfg);
    sleep_configure();
    esp_zb_set_tx_power(TX_POWER);

    /* create cluster lists for this endpoint */
    esp_zb_cluster_list_t *esp_zb_cluster_list = esp_zb_zcl_cluster_list_create();

    create_basic_cluster(esp_zb_cluster_list, firmware_version);
    create_identify_cluster(esp_zb_cluster_list);
    create_waterleak_cluster(esp_zb_cluster_list);
    create_ota_cluster(esp_zb_cluster_list,
                       OTA_UPGRADE_RUNNING_FILE_VERSION,
                       OTA_UPGRADE_DOWNLOADED_FILE_VERSION,
                       OTA_UPGRADE_MANUFACTURER,
                       OTA_UPGRADE_IMAGE_TYPE,
                       OTA_UPGRADE_HW_VERSION);


    esp_zb_ep_list_t *esp_zb_ep_list = esp_zb_ep_list_create();
    esp_zb_ep_list_add_ep(esp_zb_ep_list, esp_zb_cluster_list, SENSOR_DEVICE_ENDPOINT, ESP_ZB_AF_HA_PROFILE_ID, ESP_ZB_HA_TEMPERATURE_SENSOR_DEVICE_ID);

    esp_zb_device_register(esp_zb_ep_list); 
       
    esp_zb_core_action_handler_register(zb_action_handler);
    esp_zb_set_primary_network_channel_set(ESP_ZB_PRIMARY_CHANNEL_MASK);
    ESP_ERROR_CHECK(esp_zb_start(false));
    esp_zb_main_loop_iteration();
}

void app_main(void)
{
    esp_zb_platform_config_t config = {
        .radio_config = ESP_ZB_DEFAULT_RADIO_CONFIG(),
        .host_config = ESP_ZB_DEFAULT_HOST_CONFIG(),
    };
    
    gpio_set_direction(INPUT_PIN, GPIO_MODE_INPUT);
    gpio_set_pull_mode(INPUT_PIN, GPIO_PULLUP_ONLY);

    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_zb_platform_config(&config));
    ESP_ERROR_CHECK(esp_zb_power_save_init(CONFIG_ESP_DEFAULT_CPU_FREQ_MHZ));
    xTaskCreate(check_water_leak, "check_water_leak", configMINIMAL_STACK_SIZE * 3, NULL, 5, NULL);
    xTaskCreate(esp_zb_task, "Zigbee_main", 4096, NULL, 5, NULL);
}
