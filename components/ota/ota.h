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

#ifndef OTA_UPDATE_H
#define OTA_UPDATE_H

#include "esp_ota_ops.h"
#include "esp_timer.h"
#include "esp_log.h"
#include "esp_check.h"
#include "esp_zigbee_core.h"

#ifdef __cplusplus
extern "C" {
#endif

extern static const char *TAG_OTA;
extern static const esp_partition_t *s_ota_partition;
extern static esp_ota_handle_t s_ota_handle;
extern size_t ota_data_len_;
extern uint8_t* ota_header_;
extern size_t ota_header_size_;
extern bool ota_upgrade_subelement_;

size_t min_size_t(size_t a, size_t b);
void clear_ota_header(void);
esp_err_t zb_ota_upgrade_status_handler(esp_zb_zcl_ota_upgrade_value_message_t messsage);

#ifdef __cplusplus
}
#endif

#endif /* OTA_UPDATE_H */
