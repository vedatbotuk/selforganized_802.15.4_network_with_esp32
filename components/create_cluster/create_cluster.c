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
#include "create_cluster.h"
#include "esp_log.h"
#include "zcl/esp_zigbee_zcl_power_config.h"
#include "esp_ota_ops.h"

static char manufacturer[16] = {5, 'B', 'o', 't', 'u', '2'};
#if defined ZB_ED_ROLE
//static char model[16] = {15, 'E', 'S', 'P', '3', '2', 'H', '2', '_', 'E', 'N', 'D', '_', 'D', 'e', 'v'};
static char model[14] = {13, 'E', 'S', 'P', '3', '2', 'H', '2', '_', 'W', 'A', 'T', 'E', 'R'};
#endif
#if defined CONFIG_ZB_ZCZR
static char model[16] = {14, 'E', 'S', 'P', '3', '2', 'H', '2', '_', 'R', 'o', 'u', 't', 'e', 'r'};
#endif

RTC_DATA_ATTR uint8_t lastBatteryPercentageRemaining = 0x8C;
uint8_t test_attr;

void create_basic_cluster(esp_zb_cluster_list_t *esp_zb_cluster_list, char f_version[16])
{
    uint8_t power_source = 3;
    /* basic cluster create with fully customized */
    esp_zb_attribute_list_t *esp_zb_basic_cluster = esp_zb_zcl_attr_list_create(ESP_ZB_ZCL_CLUSTER_ID_BASIC);
    esp_zb_basic_cluster_add_attr(esp_zb_basic_cluster, ESP_ZB_ZCL_ATTR_BASIC_MANUFACTURER_NAME_ID, manufacturer);
    esp_zb_basic_cluster_add_attr(esp_zb_basic_cluster, ESP_ZB_ZCL_ATTR_BASIC_MODEL_IDENTIFIER_ID, model);
    esp_zb_basic_cluster_add_attr(esp_zb_basic_cluster, ESP_ZB_ZCL_ATTR_BASIC_SW_BUILD_ID, f_version);
    esp_zb_basic_cluster_add_attr(esp_zb_basic_cluster, ESP_ZB_ZCL_ATTR_BASIC_ZCL_VERSION_ID, &test_attr);
    esp_zb_basic_cluster_add_attr(esp_zb_basic_cluster, ESP_ZB_ZCL_ATTR_BASIC_POWER_SOURCE_ID, &power_source);
    esp_zb_cluster_update_attr(esp_zb_basic_cluster, ESP_ZB_ZCL_ATTR_BASIC_ZCL_VERSION_ID, &test_attr);
    esp_zb_cluster_list_add_basic_cluster(esp_zb_cluster_list, esp_zb_basic_cluster, ESP_ZB_ZCL_CLUSTER_SERVER_ROLE);
}

void create_identify_cluster(esp_zb_cluster_list_t *esp_zb_cluster_list)
{
    /* identify cluster create with fully customized */
    esp_zb_attribute_list_t *esp_zb_identify_cluster = esp_zb_zcl_attr_list_create(ESP_ZB_ZCL_CLUSTER_ID_IDENTIFY);
    esp_zb_identify_cluster_add_attr(esp_zb_identify_cluster, ESP_ZB_ZCL_ATTR_IDENTIFY_IDENTIFY_TIME_ID, &test_attr);
    /* create client role of the cluster */
    esp_zb_cluster_list_add_identify_cluster(esp_zb_cluster_list, esp_zb_identify_cluster, ESP_ZB_ZCL_CLUSTER_SERVER_ROLE);
}

void create_temp_cluster(esp_zb_cluster_list_t *esp_zb_cluster_list)
{
    /* Default temperature value*/
    int16_t undefined_value;
    // TODO: Are following values correct?
    uint16_t temperature_max = 5000;
    uint16_t temperature_min = -5000;
    esp_zb_attribute_list_t *esp_zb_temperature_meas_cluster = esp_zb_zcl_attr_list_create(ESP_ZB_ZCL_CLUSTER_ID_TEMP_MEASUREMENT);
    esp_zb_temperature_meas_cluster_add_attr(esp_zb_temperature_meas_cluster, ESP_ZB_ZCL_ATTR_TEMP_MEASUREMENT_VALUE_ID, &undefined_value);
    esp_zb_temperature_meas_cluster_add_attr(esp_zb_temperature_meas_cluster, ESP_ZB_ZCL_ATTR_TEMP_MEASUREMENT_MIN_VALUE_ID, &temperature_min);
    esp_zb_temperature_meas_cluster_add_attr(esp_zb_temperature_meas_cluster, ESP_ZB_ZCL_ATTR_TEMP_MEASUREMENT_MAX_VALUE_ID, &temperature_max);
    esp_zb_cluster_list_add_temperature_meas_cluster(esp_zb_cluster_list, esp_zb_temperature_meas_cluster, ESP_ZB_ZCL_CLUSTER_SERVER_ROLE);
}

void create_hum_cluster(esp_zb_cluster_list_t *esp_zb_cluster_list)
{
    uint16_t undefined_value;
    uint32_t humidity_max = 100000;
    uint16_t humidity_min = 0;
    esp_zb_attribute_list_t *esp_zb_hum_meas_cluster = esp_zb_zcl_attr_list_create(ESP_ZB_ZCL_CLUSTER_ID_REL_HUMIDITY_MEASUREMENT);
    esp_zb_humidity_meas_cluster_add_attr(esp_zb_hum_meas_cluster, ESP_ZB_ZCL_ATTR_REL_HUMIDITY_MEASUREMENT_VALUE_ID, &undefined_value);
    esp_zb_humidity_meas_cluster_add_attr(esp_zb_hum_meas_cluster, ESP_ZB_ZCL_ATTR_REL_HUMIDITY_MEASUREMENT_MIN_VALUE_ID, &humidity_min);
    esp_zb_humidity_meas_cluster_add_attr(esp_zb_hum_meas_cluster, ESP_ZB_ZCL_ATTR_REL_HUMIDITY_MEASUREMENT_MAX_VALUE_ID, &humidity_max);
    esp_zb_cluster_list_add_humidity_meas_cluster(esp_zb_cluster_list, esp_zb_hum_meas_cluster, ESP_ZB_ZCL_CLUSTER_SERVER_ROLE);
}

void create_waterleak_cluster(esp_zb_cluster_list_t *esp_zb_cluster_list)
{
//    esp_zb_metering_cluster_cfg_t metering_cluster_cfg = {
//        .metering_device_type = ESP_ZB_ZCL_METERING_WATER_METERING,
//        .status = ESP_ZB_ZCL_METERING_ALARM_CHECK_METER,
//    };
//    esp_zb_attribute_list_t *esp_zb_metering_cluster = esp_zb_metering_cluster_create(&metering_cluster_cfg);
//    esp_zb_cluster_list_add_metering_cluster(esp_zb_cluster_list, esp_zb_metering_cluster, ESP_ZB_ZCL_CLUSTER_SERVER_ROLE);
    
//    esp_zb_attribute_list_t *esp_zb_metering_cluster = esp_zb_zcl_attr_list_create(ESP_ZB_ZCL_CLUSTER_ID_METERING);
//    esp_zb_cluster_add_attr(esp_zb_metering_cluster, ESP_ZB_ZCL_CLUSTER_ID_METERING, ESP_ZB_ZCL_ATTR_METERING_STATUS_ID, ESP_ZB_ZCL_ATTR_TYPE_64BITMAP, ESP_ZB_ZCL_ATTR_ACCESS_READ_WRITE, ESP_ZB_ZCL_METERING_ALARM_CHECK_METER);
//    esp_zb_cluster_list_add_metering_cluster(esp_zb_cluster_list, esp_zb_metering_cluster, ESP_ZB_ZCL_CLUSTER_SERVER_ROLE);

    esp_zb_ias_zone_cluster_cfg_t ias_zone_cluster_cfg = {
        .zone_state = ESP_ZB_ZCL_IAS_ZONE_ZONESTATE_ENROLLED,
        .zone_type = ESP_ZB_ZCL_IAS_ZONE_ZONETYPE_WATER_SENSOR,
        .ias_cie_addr = ESP_ZB_ZCL_ZONE_IAS_CIE_ADDR_DEFAULT,
    };
    esp_zb_attribute_list_t *esp_zb_ias_zone_cluster = esp_zb_ias_zone_cluster_create(&ias_zone_cluster_cfg);
    esp_zb_cluster_list_add_ias_zone_cluster(esp_zb_cluster_list, esp_zb_ias_zone_cluster, ESP_ZB_ZCL_CLUSTER_SERVER_ROLE);

}

void create_battery_cluster(esp_zb_cluster_list_t *esp_zb_cluster_list)
{
    // TODO: add power_cluster for battery
    esp_zb_power_config_cluster_cfg_t power_cfg = {0};
    uint8_t batteryRatedVoltage = 90;
    uint8_t batteryMinVoltage = 70;
    uint8_t batteryQuantity = 1;
    uint8_t batterySize = 0x02;
    uint16_t batteryAhrRating = 50000;
    uint8_t batteryAlarmMask = 0;
    uint8_t batteryVoltage = 90;

    esp_zb_attribute_list_t *esp_zb_power_cluster = esp_zb_power_config_cluster_create(&power_cfg);
    esp_zb_power_config_cluster_add_attr(esp_zb_power_cluster, ESP_ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_VOLTAGE_ID, &batteryVoltage);
    esp_zb_power_config_cluster_add_attr(esp_zb_power_cluster, ESP_ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_SIZE_ID, &batterySize);
    esp_zb_power_config_cluster_add_attr(esp_zb_power_cluster, ESP_ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_QUANTITY_ID, &batteryQuantity);
    esp_zb_power_config_cluster_add_attr(esp_zb_power_cluster, ESP_ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_RATED_VOLTAGE_ID, &batteryRatedVoltage);
    esp_zb_power_config_cluster_add_attr(esp_zb_power_cluster, ESP_ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_ALARM_MASK_ID, &batteryAlarmMask);
    esp_zb_power_config_cluster_add_attr(esp_zb_power_cluster, ESP_ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_VOLTAGE_MIN_THRESHOLD_ID, &batteryMinVoltage);
    esp_zb_power_config_cluster_add_attr(esp_zb_power_cluster, ESP_ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_A_HR_RATING_ID, &batteryAhrRating);
    esp_zb_power_config_cluster_add_attr(esp_zb_power_cluster, ESP_ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_PERCENTAGE_REMAINING_ID, &lastBatteryPercentageRemaining);
    // remove 8 first cluster: shift the pointer to the 8th cluster
    for (int i = 0; i < 7; i++)
    {
        esp_zb_power_cluster = esp_zb_power_cluster->next;
    }
    esp_zb_cluster_list_add_power_config_cluster(esp_zb_cluster_list, esp_zb_power_cluster, ESP_ZB_ZCL_CLUSTER_SERVER_ROLE);
}

void create_ota_cluster(esp_zb_cluster_list_t *esp_zb_cluster_list,
                        uint32_t RUNNING_FILE_VERSION,
                        uint32_t DOWNLOADED_FILE_VERSION,
                        uint16_t MANUFACTURER,
                        uint16_t IMAGE_TYPE,
                        uint16_t HW_VERSION)
{
    /* OTA Upgrade Cluster*/
    esp_zb_ota_cluster_cfg_t ota_cluster_cfg = {
        .ota_upgrade_file_version = RUNNING_FILE_VERSION,
        .ota_upgrade_downloaded_file_ver = DOWNLOADED_FILE_VERSION,
        .ota_upgrade_manufacturer = MANUFACTURER,
        .ota_upgrade_image_type = IMAGE_TYPE,
    };
    esp_zb_attribute_list_t *esp_zb_ota_client_cluster = esp_zb_ota_cluster_create(&ota_cluster_cfg);
    /** add client parameters to ota client cluster */
    esp_zb_zcl_ota_upgrade_client_variable_t variable_config = {
        .timer_query = ESP_ZB_ZCL_OTA_UPGRADE_QUERY_TIMER_COUNT_DEF,
        .hw_version = HW_VERSION,
        .max_data_size = 64,
    };
    esp_zb_ota_cluster_add_attr(esp_zb_ota_client_cluster, ESP_ZB_ZCL_ATTR_OTA_UPGRADE_CLIENT_DATA_ID, (void *)&variable_config);
    esp_zb_cluster_list_add_ota_cluster(esp_zb_cluster_list, esp_zb_ota_client_cluster, ESP_ZB_ZCL_CLUSTER_CLIENT_ROLE);
}
