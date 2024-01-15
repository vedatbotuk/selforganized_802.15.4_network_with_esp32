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
 
#ifndef __DHT_H__
#define __DHT_H__

#include <driver/gpio.h>
#include <esp_err.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Sensor type
 */
typedef enum
{
    DHT_TYPE_DHT11 = 0,   //!< DHT11
    DHT_TYPE_AM2301,      //!< AM2301 (DHT21, DHT22, AM2302, AM2321)
    DHT_TYPE_SI7021       //!< Itead Si7021
} dht_sensor_type_t;

/**
 * @brief Read integer data from sensor on specified pin
 *
 * Humidity and temperature are returned as integers.
 * For example: humidity=625 is 62.5 %, temperature=244 is 24.4 degrees Celsius
 *
 * @param sensor_type DHT11 or DHT22
 * @param pin GPIO pin connected to sensor OUT
 * @param[out] humidity Humidity, percents * 10, nullable
 * @param[out] temperature Temperature, degrees Celsius * 10, nullable
 * @return `ESP_OK` on success
 */
esp_err_t dht_read_data(dht_sensor_type_t sensor_type, gpio_num_t pin,
        int16_t *humidity, int16_t *temperature);

/**
 * @brief Read float data from sensor on specified pin
 *
 * Humidity and temperature are returned as floats.
 *
 * @param sensor_type DHT11 or DHT22
 * @param pin GPIO pin connected to sensor OUT
 * @param[out] humidity Humidity, percents, nullable
 * @param[out] temperature Temperature, degrees Celsius, nullable
 * @return `ESP_OK` on success
 */
esp_err_t dht_read_float_data(dht_sensor_type_t sensor_type, gpio_num_t pin,
        float *humidity, float *temperature);

#ifdef __cplusplus
}
#endif

/**@}*/

#endif  // __DHT_H__
