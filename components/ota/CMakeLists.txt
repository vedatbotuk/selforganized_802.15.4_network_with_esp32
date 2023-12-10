idf_component_register(
    SRCS ota.c
    INCLUDE_DIRS .
    PRIV_REQUIRES app_update esp_timer espressif__esp-zboss-lib driver espressif__esp-zigbee-lib
)
