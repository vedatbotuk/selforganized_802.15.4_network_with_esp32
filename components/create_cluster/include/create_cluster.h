/*
 * selforganized_802.15.4_network_with_esp32
 * Copyright (c) 2024 Vedat Botuk.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef CREATE_CLUSTERS_H
#define CREATE_CLUSTERS_H

#include "esp_zigbee_core.h"

#ifdef __cplusplus
extern "C"
{
#endif

    void create_basic_cluster(esp_zb_cluster_list_t *esp_zb_cluster_list, char f_version[16]);
    void create_identify_cluster(esp_zb_cluster_list_t *esp_zb_cluster_list);
    void create_temp_cluster(esp_zb_cluster_list_t *esp_zb_cluster_list);
    void create_hum_cluster(esp_zb_cluster_list_t *esp_zb_cluster_list);
    void create_waterleak_cluster(esp_zb_cluster_list_t *esp_zb_cluster_list);
    void create_battery_cluster(esp_zb_cluster_list_t *esp_zb_cluster_list);
    void create_ota_cluster(esp_zb_cluster_list_t *esp_zb_cluster_list);

#ifdef __cplusplus
}
#endif

#endif /* CREATE_CLUSTERS_H */
