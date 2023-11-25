/*
 * SPDX-FileCopyrightText: 2021-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 *
 * Zigbee HA_TEMPERATURE
 *
 * This example code is in the Public Domain (or CC0 licensed, at your option.)
 *
 * Unless required by applicable law or agreed to in writing, this
 * software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
 * CONDITIONS OF ANY KIND, either express or implied.
 */
#include "esp_zb_temperature.h"
#include "esp_check.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "ha/esp_zigbee_ha_standard.h"
#include <dht22.h>
#include "driver/gpio.h"
#include "battery_read.c"

#if !defined ZB_ED_ROLE
#error Define ZB_ED_ROLE in idf.py menuconfig to compile light (End Device) source code.
#endif

#if CONFIG_IDF_TARGET_ESP32C6
#define CONFIG_EXAMPLE_DATA_GPIO GPIO_NUM_2
#elif CONFIG_IDF_TARGET_ESP32H2
#define CONFIG_EXAMPLE_DATA_GPIO GPIO_NUM_0
#endif

#define SENSOR_TYPE DHT_TYPE_AM2301

static char manufacturer[16] = {5, 'B', 'o', 't', 'u', 'k'};
static char model[16] = {15, 'E', 'S', 'P', '3', '2', 'H', '2', ' ', 'E', 'N', 'D', ' ', 'D', 'e', 'v'};
static char firmware_version[16] = {6, 'v', 'e', 'r', '0', '.', '1'};
static const char *TAG = "ESP_ZB_TEMPERATURE";
bool connected = false;

/********************* Define functions **************************/
static void bdb_start_top_level_commissioning_cb(uint8_t mode_mask)
{
    ESP_ERROR_CHECK(esp_zb_bdb_start_top_level_commissioning(mode_mask));
}

void zb_update_temp(int temperature)
{
    static esp_zb_zcl_report_attr_cmd_t temp_measurement_cmd_req = {};
        temp_measurement_cmd_req.zcl_basic_cmd.src_endpoint = HA_ESP_TEMPERATURE_ENDPOINT;
        temp_measurement_cmd_req.address_mode = ESP_ZB_APS_ADDR_MODE_DST_ADDR_ENDP_NOT_PRESENT;
        temp_measurement_cmd_req.clusterID = ESP_ZB_ZCL_CLUSTER_ID_TEMP_MEASUREMENT;
        temp_measurement_cmd_req.cluster_role = ESP_ZB_ZCL_CLUSTER_SERVER_ROLE;

    /* Write new temp */
    esp_zb_zcl_status_t state = esp_zb_zcl_set_attribute_val(HA_ESP_TEMPERATURE_ENDPOINT, ESP_ZB_ZCL_CLUSTER_ID_TEMP_MEASUREMENT, ESP_ZB_ZCL_CLUSTER_SERVER_ROLE, ESP_ZB_ZCL_ATTR_TEMP_MEASUREMENT_VALUE_ID, &temperature, false);

    /* Check for error */
    if(state != ESP_ZB_ZCL_STATUS_SUCCESS) {
        ESP_LOGE(TAG, "Setting temp attribute failed!");
        return;
    }

    /* Request sending new phase voltage */
    esp_err_t state1 = esp_zb_zcl_report_attr_cmd_req(&temp_measurement_cmd_req);

    /* Check for error */
    if(state1 != ESP_OK) {
        ESP_LOGE(TAG, "Sending temp attribute report command failed!");
        return;
    }
    ESP_LOGI(TAG, "Setting temp success");
    return;
}

void measure_temperature()
{
    uint16_t temperature_to_send = 0;
    uint16_t temperature_max = 50000;
    uint16_t temperature_min = -1000;
    float temperature, humidity;
    uint16_t temp_temperature = 0;
    int battery_level;
    
    /* Set min/max temperature values */
    while (1) {
        if (connected){
            if (dht_read_float_data(SENSOR_TYPE, CONFIG_EXAMPLE_DATA_GPIO, &humidity, &temperature) == ESP_OK){
                ESP_LOGI(TAG, "Temperature : %.1f ℃", temperature); 
                temperature_to_send = (uint16_t) (temperature * 100);
                ESP_LOGI(TAG, "Temperature write attribute first time, after start.");
                esp_zb_zcl_set_attribute_val(HA_ESP_TEMPERATURE_ENDPOINT, ESP_ZB_ZCL_CLUSTER_ID_TEMP_MEASUREMENT, ESP_ZB_ZCL_CLUSTER_SERVER_ROLE, ESP_ZB_ZCL_ATTR_TEMP_MEASUREMENT_VALUE_ID, &temperature_to_send, false);
                esp_zb_zcl_set_attribute_val(HA_ESP_TEMPERATURE_ENDPOINT, ESP_ZB_ZCL_CLUSTER_ID_TEMP_MEASUREMENT, ESP_ZB_ZCL_CLUSTER_SERVER_ROLE, ESP_ZB_ZCL_ATTR_TEMP_MEASUREMENT_MAX_VALUE_ID, &temperature_max, false); 
                esp_zb_zcl_set_attribute_val(HA_ESP_TEMPERATURE_ENDPOINT, ESP_ZB_ZCL_CLUSTER_ID_TEMP_MEASUREMENT, ESP_ZB_ZCL_CLUSTER_SERVER_ROLE, ESP_ZB_ZCL_ATTR_TEMP_MEASUREMENT_MIN_VALUE_ID, &temperature_min, false);
                break;
            } else {
                ESP_LOGW(TAG, "Could not read data from sensor.");
            }

        } else {
            ESP_LOGI(TAG, "Device is not connected!");
        }
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
    
    /* Wait for next mesurement */
    vTaskDelay(pdMS_TO_TICKS(5000));
    
    /* Measure temperature loop*/
    while (1) {
        if (connected){
            if (dht_read_float_data(SENSOR_TYPE, CONFIG_EXAMPLE_DATA_GPIO, &humidity, &temperature) == ESP_OK){
                ESP_LOGI(TAG, "Temperature : %.1f ℃", temperature); 
                temperature_to_send = (uint16_t) (temperature * 100);
                if (temperature_to_send != temp_temperature) {
                    ESP_LOGI(TAG, "Temperature changes, will report new value");
                    zb_update_temp(temperature_to_send);
                    battery_level = get_battery_level();
                    ESP_LOGI(TAG, "Battery level: %d %%", battery_level);
                    temp_temperature = temperature_to_send;
                } else {
                    ESP_LOGI(TAG, "Temperature is the same, will not report.");
                }
            } else {
                ESP_LOGW(TAG, "Could not read data from sensor.");
            }
        } else {
            ESP_LOGI(TAG, "Device is not connected!");
        }
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}

void esp_zb_app_signal_handler(esp_zb_app_signal_t *signal_struct)
{
    uint32_t *p_sg_p = signal_struct->p_app_signal;
    esp_err_t err_status = signal_struct->esp_err_status;
    esp_zb_app_signal_type_t sig_type = *p_sg_p;
    switch (sig_type) {
    case ESP_ZB_ZDO_SIGNAL_SKIP_STARTUP:
        ESP_LOGI(TAG, "Zigbee stack initialized");
        esp_zb_bdb_start_top_level_commissioning(ESP_ZB_BDB_MODE_INITIALIZATION);
        break;
    case ESP_ZB_BDB_SIGNAL_DEVICE_FIRST_START:
    case ESP_ZB_BDB_SIGNAL_DEVICE_REBOOT:
        if (err_status == ESP_OK) {
            connected = true;
            ESP_LOGI(TAG, "Start network steering");
            esp_zb_bdb_start_top_level_commissioning(ESP_ZB_BDB_MODE_NETWORK_STEERING);
        } else {
            connected = false;
            ESP_LOGW(TAG, "Failed to initialize Zigbee stack (status: %s)", esp_err_to_name(err_status));
        }
        break;
    case ESP_ZB_BDB_SIGNAL_STEERING:
        if (err_status == ESP_OK) {
            esp_zb_ieee_addr_t extended_pan_id;
            esp_zb_get_extended_pan_id(extended_pan_id);
            ESP_LOGI(TAG, "Joined network successfully (Extended PAN ID: %02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x, PAN ID: 0x%04hx, Channel:%d)",
                     extended_pan_id[7], extended_pan_id[6], extended_pan_id[5], extended_pan_id[4],
                     extended_pan_id[3], extended_pan_id[2], extended_pan_id[1], extended_pan_id[0],
                     esp_zb_get_pan_id(), esp_zb_get_current_channel());
        } else {
            ESP_LOGI(TAG, "Network steering was not successful (status: %s)", esp_err_to_name(err_status));
            esp_zb_scheduler_alarm((esp_zb_callback_t)bdb_start_top_level_commissioning_cb, ESP_ZB_BDB_MODE_NETWORK_STEERING, 1000);
        }
        break;
    default:
        ESP_LOGI(TAG, "ZDO signal: %s (0x%x), status: %s", esp_zb_zdo_signal_to_string(sig_type), sig_type, esp_err_to_name(err_status));
        break;
    }
}

static void esp_zb_task(void *pvParameters)
{
    /* initialize Zigbee stack */
    esp_zb_cfg_t zb_nwk_cfg = ESP_ZB_ZED_CONFIG();
    esp_zb_init(&zb_nwk_cfg);
    /* Set trasmitter power tx_power(0) = -24dB */
//    esp_zb_set_tx_power(10);

    uint8_t test_attr;
    test_attr = 0;
    /* basic cluster create with fully customized */
    esp_zb_attribute_list_t *esp_zb_basic_cluster = esp_zb_zcl_attr_list_create(ESP_ZB_ZCL_CLUSTER_ID_BASIC);
    esp_zb_basic_cluster_add_attr(esp_zb_basic_cluster, ESP_ZB_ZCL_ATTR_BASIC_MANUFACTURER_NAME_ID, manufacturer);
    esp_zb_basic_cluster_add_attr(esp_zb_basic_cluster, ESP_ZB_ZCL_ATTR_BASIC_MODEL_IDENTIFIER_ID, model);
    esp_zb_basic_cluster_add_attr(esp_zb_basic_cluster, ESP_ZB_ZCL_ATTR_BASIC_SW_BUILD_ID, firmware_version);
    esp_zb_basic_cluster_add_attr(esp_zb_basic_cluster, ESP_ZB_ZCL_ATTR_BASIC_ZCL_VERSION_ID, &test_attr);
    esp_zb_basic_cluster_add_attr(esp_zb_basic_cluster, ESP_ZB_ZCL_ATTR_BASIC_POWER_SOURCE_ID, &test_attr);
    esp_zb_cluster_update_attr(esp_zb_basic_cluster, ESP_ZB_ZCL_ATTR_BASIC_ZCL_VERSION_ID, &test_attr);
    /* identify cluster create with fully customized */
    esp_zb_attribute_list_t *esp_zb_identify_cluster = esp_zb_zcl_attr_list_create(ESP_ZB_ZCL_CLUSTER_ID_IDENTIFY);
    esp_zb_identify_cluster_add_attr(esp_zb_identify_cluster, ESP_ZB_ZCL_ATTR_IDENTIFY_IDENTIFY_TIME_ID, &test_attr);
    /* create client role of the cluster */

    uint16_t undefined_value;
    undefined_value = 0x8000;
    /* Temperature cluster */
    esp_zb_attribute_list_t *esp_zb_temperature_meas_cluster = esp_zb_zcl_attr_list_create(ESP_ZB_ZCL_CLUSTER_ID_TEMP_MEASUREMENT);
    esp_zb_temperature_meas_cluster_add_attr(esp_zb_temperature_meas_cluster, ESP_ZB_ZCL_ATTR_TEMP_MEASUREMENT_VALUE_ID, &undefined_value);
    esp_zb_temperature_meas_cluster_add_attr(esp_zb_temperature_meas_cluster, ESP_ZB_ZCL_ATTR_TEMP_MEASUREMENT_MIN_VALUE_ID, &undefined_value);
    esp_zb_temperature_meas_cluster_add_attr(esp_zb_temperature_meas_cluster, ESP_ZB_ZCL_ATTR_TEMP_MEASUREMENT_MAX_VALUE_ID, &undefined_value);

    /* create cluster lists for this endpoint */
    esp_zb_cluster_list_t *esp_zb_cluster_list = esp_zb_zcl_cluster_list_create();
    esp_zb_cluster_list_add_basic_cluster(esp_zb_cluster_list, esp_zb_basic_cluster, ESP_ZB_ZCL_CLUSTER_SERVER_ROLE);
    esp_zb_cluster_list_add_identify_cluster(esp_zb_cluster_list, esp_zb_identify_cluster, ESP_ZB_ZCL_CLUSTER_SERVER_ROLE);
    esp_zb_cluster_list_add_temperature_meas_cluster(esp_zb_cluster_list, esp_zb_temperature_meas_cluster, ESP_ZB_ZCL_CLUSTER_SERVER_ROLE);

    esp_zb_ep_list_t *esp_zb_ep_list = esp_zb_ep_list_create();
    esp_zb_ep_list_add_ep(esp_zb_ep_list, esp_zb_cluster_list, HA_ESP_TEMPERATURE_ENDPOINT, ESP_ZB_AF_HA_PROFILE_ID, ESP_ZB_HA_TEMPERATURE_SENSOR_DEVICE_ID);
    
    esp_zb_device_register(esp_zb_ep_list);
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
    
    voltage_calculate_init();
    
    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_zb_platform_config(&config));
    xTaskCreate(measure_temperature, "measure_temperature", configMINIMAL_STACK_SIZE * 3, NULL, 5, NULL);
    xTaskCreate(esp_zb_task, "Zigbee_main", 4096, NULL, 6, NULL);
}
