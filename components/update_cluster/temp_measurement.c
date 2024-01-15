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

#include <stdint.h>
#include "esp_zigbee_core.h"
#include "zcl/esp_zigbee_zcl_power_config.h"
#include "esp_log.h"

static const char *TAG = "UPDATE_TEMP_CLUSTER";

void zb_update_temp(int16_t temperature, uint8_t endpoint)
{
    static esp_zb_zcl_report_attr_cmd_t temp_measurement_cmd_req = {};
    temp_measurement_cmd_req.zcl_basic_cmd.src_endpoint = endpoint;
    temp_measurement_cmd_req.address_mode = ESP_ZB_APS_ADDR_MODE_DST_ADDR_ENDP_NOT_PRESENT;
    temp_measurement_cmd_req.clusterID = ESP_ZB_ZCL_CLUSTER_ID_TEMP_MEASUREMENT;
    temp_measurement_cmd_req.cluster_role = ESP_ZB_ZCL_CLUSTER_SERVER_ROLE;

    esp_zb_zcl_status_t state = esp_zb_zcl_set_attribute_val(endpoint, ESP_ZB_ZCL_CLUSTER_ID_TEMP_MEASUREMENT, ESP_ZB_ZCL_CLUSTER_SERVER_ROLE, ESP_ZB_ZCL_ATTR_TEMP_MEASUREMENT_VALUE_ID, &temperature, false);
    if (state != ESP_OK)
    {
        ESP_LOGE(TAG, "Sending temp attribute report command failed!");
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
