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

#include "esp_zigbee_core.h"

/* Zigbee configuration */
#define INSTALLCODE_POLICY_ENABLE false                /* enable the install code policy for security */
#define ED_AGING_TIMEOUT ESP_ZB_ED_AGING_TIMEOUT_64MIN /* End device ages time */
#define ESP_ZB_PRIMARY_CHANNEL_MASK (1l << 25)         /* Zigbee primary channel mask use in the example */

#define ESP_ZB_ZED_CONFIG()                               \
    {                                                     \
        .esp_zb_role = ESP_ZB_DEVICE_TYPE_ED,             \
        .install_code_policy = INSTALLCODE_POLICY_ENABLE, \
        .nwk_cfg.zed_cfg = {                              \
            .ed_timeout = ED_AGING_TIMEOUT,               \
            .keep_alive = ED_KEEP_ALIVE,                  \
        },                                                \
    }

#define ESP_ZB_DEFAULT_RADIO_CONFIG()    \
    {                                    \
        .radio_mode = RADIO_MODE_NATIVE, \
    }

#define ESP_ZB_DEFAULT_HOST_CONFIG()                       \
    {                                                      \
        .host_connection_mode = HOST_CONNECTION_MODE_NONE, \
    }
