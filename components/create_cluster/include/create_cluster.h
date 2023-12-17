#ifndef CREATE_CLUSTERS_H
#define CREATE_CLUSTERS_H

#include <stdio.h>
#include "create_cluster.h"
#include "esp_zigbee_core.h"
#include "esp_log.h"

#ifdef __cplusplus
extern "C"
{
#endif

    void create_basic_cluster(esp_zb_cluster_list_t *esp_zb_cluster_list, char f_version[16]);
    void create_identify_cluster(esp_zb_cluster_list_t *esp_zb_cluster_list);
    void create_temp_cluster(esp_zb_cluster_list_t *esp_zb_cluster_list);
    void create_hum_cluster(esp_zb_cluster_list_t *esp_zb_cluster_list);
    void create_battery_cluster(esp_zb_cluster_list_t *esp_zb_cluster_list);
    void create_ota_cluster(esp_zb_cluster_list_t *esp_zb_cluster_list,
                            uint32_t RUNNING_FILE_VERSION,
                            uint32_t DOWNLOADED_FILE_VERSION,
                            uint16_t MANUFACTURER,
                            uint16_t IMAGE_TYPE,
                            uint16_t HW_VERSION);

#ifdef __cplusplus
}
#endif

#endif /* CREATE_CLUSTERS_H */
