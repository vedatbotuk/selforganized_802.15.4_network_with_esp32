idf_component_register(
    SRCS "create_cluster.c"
    INCLUDE_DIRS "include"
    PRIV_REQUIRES espressif__esp-zigbee-lib driver app_update
    )
