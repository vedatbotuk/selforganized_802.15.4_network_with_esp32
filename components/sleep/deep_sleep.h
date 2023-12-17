/*
 * SPDX-FileCopyrightText: 2022-2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 *
 * Zigbee Sleepy end device Example
 *
 * This example code is in the Public Domain (or CC0 licensed, at your option.)
 *
 * Unless required by applicable law or agreed to in writing, this
 * software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
 * CONDITIONS OF ANY KIND, either express or implied.
 */

#ifndef ZIGBEE_SLEEPY_END_DEVICE_H
#define ZIGBEE_SLEEPY_END_DEVICE_H

#include "esp_check.h"
#include "nvs_flash.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "time.h"
#include "sys/time.h"
#include "driver/rtc_io.h"
#include "driver/uart.h"
#include "esp_sleep.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "ha/esp_zigbee_ha_standard.h"

#ifdef CONFIG_PM_ENABLE
#include "esp_pm.h"
#include "esp_private/esp_clk.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

extern const char *TAG_SLEEP;
extern RTC_DATA_ATTR struct timeval s_sleep_enter_time;
extern esp_timer_handle_t s_oneshot_timer;

void zb_deep_sleep_init(void);

#ifdef __cplusplus
}
#endif

#endif /* ZIGBEE_SLEEPY_END_DEVICE_H */
