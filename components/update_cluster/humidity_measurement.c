#include <stdint.h>
#include "esp_zigbee_core.h"
#include "zcl/esp_zigbee_zcl_power_config.h"
#include "esp_log.h"

static const char *TAG = "UPDATE_HUMIDITY_CLUSTER";

void zb_update_hum(int humidity, uint8_t endpoint)
{
    static esp_zb_zcl_report_attr_cmd_t humidity_measurement_cmd_req = {};
    humidity_measurement_cmd_req.zcl_basic_cmd.src_endpoint = endpoint;
    humidity_measurement_cmd_req.address_mode = ESP_ZB_APS_ADDR_MODE_DST_ADDR_ENDP_NOT_PRESENT;
    humidity_measurement_cmd_req.clusterID = ESP_ZB_ZCL_CLUSTER_ID_REL_HUMIDITY_MEASUREMENT;
    humidity_measurement_cmd_req.attributeID = ESP_ZB_ZCL_ATTR_REL_HUMIDITY_MEASUREMENT_VALUE_ID;
    humidity_measurement_cmd_req.cluster_role = ESP_ZB_ZCL_CLUSTER_SERVER_ROLE;

    /* Write new temp */
    esp_zb_zcl_status_t state = esp_zb_zcl_set_attribute_val(endpoint, ESP_ZB_ZCL_CLUSTER_ID_REL_HUMIDITY_MEASUREMENT, ESP_ZB_ZCL_CLUSTER_SERVER_ROLE, ESP_ZB_ZCL_ATTR_REL_HUMIDITY_MEASUREMENT_VALUE_ID, &humidity, false);
    /* Check for error */
    if (state != ESP_ZB_ZCL_STATUS_SUCCESS)
    {
        ESP_LOGE(TAG, "Setting hum attribute failed!");
        return;
    }

    esp_err_t state1 = esp_zb_zcl_report_attr_cmd_req(&humidity_measurement_cmd_req);
    /* Check for error */
    if(state1 != ESP_ZB_ZCL_STATUS_SUCCESS) {
        ESP_LOGE(TAG, "Setting hum attribute failed!");
        return;
    }

    ESP_LOGI(TAG, "Setting hum success");
    return;
}
