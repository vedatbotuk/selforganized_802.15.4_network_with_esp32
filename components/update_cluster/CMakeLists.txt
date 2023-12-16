idf_component_register(
    SRCS battery_measurement.c humidity_measurement.c temp_measurement.c
    INCLUDE_DIRS .
    PRIV_REQUIRES espressif__esp-zigbee-lib driver
)
