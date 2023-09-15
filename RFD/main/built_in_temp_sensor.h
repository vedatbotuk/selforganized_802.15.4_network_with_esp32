#ifndef built_in_temp_sensor_DOT_H    /* This is an "include guard" */
#define built_in_temp_sensor_DOT_H    /* prevents the file from being included twice. */
                     /* Including a header file twice causes all kinds */
                     /* of interesting problems.*/

#include "driver/temperature_sensor.h"
#include "esp_attr.h"

//static const char *TAG = "ESP_TEMPERATURE";


void built_in_temp_sensor_configure();

int built_in_sensor_get_temp();

#endif /* built_in_temp_sensor_DOT_H */
