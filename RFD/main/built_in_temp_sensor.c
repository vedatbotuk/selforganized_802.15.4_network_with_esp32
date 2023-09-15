#include "built_in_temp_sensor.h"
#include "driver/temperature_sensor.h"
#include "esp_attr.h"
#include "esp_log.h"

static const char *TAG = "ESP_TEMPERATURE";

IRAM_ATTR static bool temp_sensor_monitor_cbs(temperature_sensor_handle_t tsens, const temperature_sensor_threshold_event_data_t *edata, void *user_data)

{
    ESP_DRAM_LOGI("tsens", "Temperature value is higher or lower than threshold, value is %d\n...\n\n", edata->celsius_value);
    return false;
}

float tsens_value;
temperature_sensor_handle_t temp_sensor = NULL;

void built_in_temp_sensor_configure(void){

        
        temperature_sensor_config_t temp_sensor_config = TEMPERATURE_SENSOR_CONFIG_DEFAULT(10, 50);
        temperature_sensor_install(&temp_sensor_config, &temp_sensor);


        temperature_sensor_event_callbacks_t cbs = {
            .on_threshold = temp_sensor_monitor_cbs,
        };

        temperature_sensor_abs_threshold_config_t threshold_cfg = {
            .high_threshold = 50,
            .low_threshold = -10,
        };
        temperature_sensor_set_absolute_threshold(temp_sensor, &threshold_cfg);

        temperature_sensor_register_callbacks(temp_sensor, &cbs, NULL);

        ESP_LOGI(TAG, "Enable temperature sensor");
        temperature_sensor_enable(temp_sensor);

        ESP_LOGI(TAG, "Read temperature");
    

}


int built_in_sensor_get_temp(int){
        temperature_sensor_get_celsius(temp_sensor, &tsens_value);
        return tsens_value;
}
