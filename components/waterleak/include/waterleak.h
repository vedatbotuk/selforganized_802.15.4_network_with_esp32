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

#ifndef WATERLEAK_H
#define WATERLEAK_H

#include "driver/gpio.h"
#include <esp_err.h>
// #include "update_cluster.h"

#ifdef __cplusplus
extern "C"
{
#endif

    esp_err_t check_waterleak(void);
    esp_err_t button_init(void);

#ifdef __cplusplus
}
#endif

#endif /* WATERLEAK_H */
