/*
 * SPDX-FileCopyrightText: 2021-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 *
 * Zigbee HA_TEMPERATURE
 *
 * This example code is in the Public Domain (or CC0 licensed, at your option.)
 *
 * Unless required by applicable law or agreed to in writing, this
 * software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
 * CONDITIONS OF ANY KIND, either express or implied.
 */

#include "esp_zigbee_core.h"

/* Zigbee configuration */
#define INSTALLCODE_POLICY_ENABLE           false    /* enable the install code policy for security */
#define ED_AGING_TIMEOUT                    ESP_ZB_ED_AGING_TIMEOUT_64MIN          /* End device ages time */
#define ED_KEEP_ALIVE                       4000    /* 3000 millisecond */
#define ESP_OTA_CLIENT_ENDPOINT             5                                       /* OTA endpoint identifier */
#define OTA_UPGRADE_MANUFACTURER            0x1001                                  /* The attribute indicates the file version of the downloaded image on the device*/
#define OTA_UPGRADE_IMAGE_TYPE              0x1011                                  /* The attribute indicates the value for the manufacturer of the device */
#define OTA_UPGRADE_RUNNING_FILE_VERSION    0x01010105                              /* The attribute indicates the file version of the running firmware image on the device */
#define OTA_UPGRADE_DOWNLOADED_FILE_VERSION 0x01010105                              /* The attribute indicates the file version of the downloaded firmware image on the device */
#define OTA_UPGRADE_HW_VERSION              0x0101                                  /* The parameter indicates the version of hardware */
#define SENSOR_DEVICE_ENDPOINT              10    /* esp light bulb device endpoint, used to process light controlling commands */
#define ESP_ZB_PRIMARY_CHANNEL_MASK         (1l << 25)  /* Zigbee primary channel mask use in the example */
#define TX_POWER 5                                      /* Set TX Power (min=0 - max=15). Set trasmitter power tx_power(0) = -24dB*/

#define ESP_ZB_ZED_CONFIG()                                         \
    {                                                               \
        .esp_zb_role = ESP_ZB_DEVICE_TYPE_ED,                       \
        .install_code_policy = INSTALLCODE_POLICY_ENABLE,           \
        .nwk_cfg.zed_cfg = {                                        \
            .ed_timeout = ED_AGING_TIMEOUT,                         \
            .keep_alive = ED_KEEP_ALIVE,                            \
        },                                                          \
    }

#define ESP_ZB_DEFAULT_RADIO_CONFIG()                           \
    {                                                           \
        .radio_mode = RADIO_MODE_NATIVE,                        \
    }

#define ESP_ZB_DEFAULT_HOST_CONFIG()                            \
    {                                                           \
        .host_connection_mode = HOST_CONNECTION_MODE_NONE,      \
    }
