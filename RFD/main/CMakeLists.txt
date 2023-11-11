idf_component_register(
    SRCS
    "esp_zb_temperature.c"
    "dht22.c"
    "esp_idf_lib_helpers.h"
    "ets_sys.h"
    "battery_read.c"
    INCLUDE_DIRS "."
)
