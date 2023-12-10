idf_component_register(
    SRCS dht22.c
    INCLUDE_DIRS .
    PRIV_REQUIRES esp_driver_gpio ets_sys esp_idf_lib_helpers
)
