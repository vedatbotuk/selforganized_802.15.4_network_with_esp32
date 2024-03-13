idf_component_register(
    SRCS "signal_handler.c"
    INCLUDE_DIRS .
    PRIV_REQUIRES esp_timer espressif__esp-zigbee-lib driver sleep nvs_flash battery esp_adc waterleak
    )
