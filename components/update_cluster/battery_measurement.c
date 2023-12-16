void zb_update_battery_level(int level, int voltage)
{
    static esp_zb_zcl_report_attr_cmd_t battery_level_measurement_cmd_req = {};
    battery_level_measurement_cmd_req.zcl_basic_cmd.src_endpoint = SENSOR_DEVICE_ENDPOINT;
    battery_level_measurement_cmd_req.address_mode = ESP_ZB_APS_ADDR_MODE_DST_ADDR_ENDP_NOT_PRESENT;
    battery_level_measurement_cmd_req.clusterID = ESP_ZB_ZCL_CLUSTER_ID_POWER_CONFIG;
    battery_level_measurement_cmd_req.attributeID = ESP_ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_PERCENTAGE_REMAINING_ID;
    battery_level_measurement_cmd_req.cluster_role = ESP_ZB_ZCL_CLUSTER_SERVER_ROLE;

    /* Write new level */
    esp_zb_zcl_status_t state_level = esp_zb_zcl_set_attribute_val(SENSOR_DEVICE_ENDPOINT, ESP_ZB_ZCL_CLUSTER_ID_POWER_CONFIG,ESP_ZB_ZCL_CLUSTER_SERVER_ROLE,  ESP_ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_PERCENTAGE_REMAINING_ID, &level, false);
    esp_zb_zcl_status_t state_voltage = esp_zb_zcl_set_attribute_val(SENSOR_DEVICE_ENDPOINT, ESP_ZB_ZCL_CLUSTER_ID_POWER_CONFIG, ESP_ZB_ZCL_CLUSTER_SERVER_ROLE, ESP_ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_VOLTAGE_ID, &voltage, false);

    /* Check for error */
    if(state_level != ESP_ZB_ZCL_STATUS_SUCCESS) {
        ESP_LOGE(TAG, "Setting battery level attribute failed!");
        return;
    }
    
    if (state_voltage != ESP_ZB_ZCL_STATUS_SUCCESS)
    {
        ESP_LOGE(TAG, "Setting battery voltage attribute failed!");
        return;
    }

    esp_err_t state1 = esp_zb_zcl_report_attr_cmd_req(&battery_level_measurement_cmd_req);
    /* Check for error */
    if (state1 != ESP_ZB_ZCL_STATUS_SUCCESS)
    {
        ESP_LOGE(TAG, "Setting battery attributes failed!");
        return;
    }

    ESP_LOGI(TAG, "Setting battery level success");
    return;
}
