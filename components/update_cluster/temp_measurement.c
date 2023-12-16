void zb_update_temp(int temperature)
{
    static esp_zb_zcl_report_attr_cmd_t temp_measurement_cmd_req = {};
    temp_measurement_cmd_req.zcl_basic_cmd.src_endpoint = SENSOR_DEVICE_ENDPOINT;
    temp_measurement_cmd_req.address_mode = ESP_ZB_APS_ADDR_MODE_DST_ADDR_ENDP_NOT_PRESENT;
    temp_measurement_cmd_req.clusterID = ESP_ZB_ZCL_CLUSTER_ID_TEMP_MEASUREMENT;
    temp_measurement_cmd_req.cluster_role = ESP_ZB_ZCL_CLUSTER_SERVER_ROLE;

    esp_zb_zcl_status_t state = esp_zb_zcl_set_attribute_val(SENSOR_DEVICE_ENDPOINT, ESP_ZB_ZCL_CLUSTER_ID_TEMP_MEASUREMENT, ESP_ZB_ZCL_CLUSTER_SERVER_ROLE, ESP_ZB_ZCL_ATTR_TEMP_MEASUREMENT_VALUE_ID, &temperature, false);
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
