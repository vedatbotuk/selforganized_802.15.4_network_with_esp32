/* Mesh Manual Networking Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include <string.h>
#include "esp_err.h"
#include "esp_mesh.h"
#include "mesh_light.h"
#include "driver/gpio.h"
#include "led_strip.h"
#include <stdio.h>


/*******************************************************
 *                Constants
 *******************************************************/
#define BLINK_GPIO CONFIG_BLINK_GPIO

/*******************************************************
 *                Variable Definitions
 *******************************************************/
static bool s_light_inited = false;

static led_strip_handle_t led_strip;

/*******************************************************
 *                Function Definitions
 *******************************************************/
esp_err_t mesh_light_init(void)
{
    if (s_light_inited == true) {
        return ESP_OK;
    }
    s_light_inited = true;


#ifdef CONFIG_IDF_TARGET_ESP32
    ledc_timer.speed_mode = LEDC_HIGH_SPEED_MODE;
#endif
    led_strip_rmt_config_t ledc_timer = {
        .resolution_hz = 10 * 1000 * 1000, // 10MHz
    };

    led_strip_config_t ledc_channel = {
        .strip_gpio_num = BLINK_GPIO,
        .max_leds = 1, // at least one LED on board
    };
    
    mesh_light_set(MESH_LIGHT_INIT);
    led_strip_new_rmt_device(&ledc_channel, &ledc_timer, &led_strip);
    led_strip_clear(led_strip);
    return ESP_OK;
}

esp_err_t mesh_light_set(int color)
{
    switch (color) {
    case MESH_LIGHT_RED:
        /* Red */
        led_strip_set_pixel(led_strip, 0, 16, 0, 0);
        led_strip_refresh(led_strip);
        break;
    case MESH_LIGHT_GREEN:
        /* Green */
        led_strip_set_pixel(led_strip, 0, 0, 16, 0);
        led_strip_refresh(led_strip);
        break;
    case MESH_LIGHT_BLUE:
        /* Blue */
        led_strip_set_pixel(led_strip, 0, 0, 0, 16);
        led_strip_refresh(led_strip);
        break;
    case MESH_LIGHT_YELLOW:
        /* Yellow */
        led_strip_set_pixel(led_strip, 0, 16, 16, 0);
        led_strip_refresh(led_strip);
        break;
    case MESH_LIGHT_PINK:
        /* Pink */
        led_strip_set_pixel(led_strip, 0, 16, 0, 16);
        led_strip_refresh(led_strip);
        break;
    case MESH_LIGHT_INIT:
        /* can't say */
        led_strip_set_pixel(led_strip, 0, 0, 16, 16);
        led_strip_refresh(led_strip);
        break;
    case MESH_LIGHT_WARNING:
        /* warning */
        led_strip_set_pixel(led_strip, 0, 16, 16, 16);
        led_strip_refresh(led_strip);
        break;
    default:
        /* off */
        led_strip_clear(led_strip);
    }

    return ESP_OK;
}

void mesh_connected_indicator(int layer)
{
    switch (layer) {
    case 1:
        mesh_light_set(MESH_LIGHT_PINK);
        break;
    case 2:
        mesh_light_set(MESH_LIGHT_YELLOW);
        break;
    case 3:
        mesh_light_set(MESH_LIGHT_RED);
        break;
    case 4:
        mesh_light_set(MESH_LIGHT_BLUE);
        break;
    case 5:
        mesh_light_set(MESH_LIGHT_GREEN);
        break;
    case 6:
        mesh_light_set(MESH_LIGHT_WARNING);
        break;
    default:
        mesh_light_set(0);
    }
}

void mesh_disconnected_indicator(void)
{
    mesh_light_set(MESH_LIGHT_WARNING);
}

esp_err_t mesh_light_process(mesh_addr_t *from, uint8_t *buf, uint16_t len)
{
    mesh_light_ctl_t *in = (mesh_light_ctl_t *) buf;
    if (!from || !buf || len < sizeof(mesh_light_ctl_t)) {
        return ESP_FAIL;
    }
    if (in->token_id != MESH_TOKEN_ID || in->token_value != MESH_TOKEN_VALUE) {
        return ESP_FAIL;
    }
    if (in->cmd == MESH_CONTROL_CMD) {
        if (in->on) {
            mesh_connected_indicator(esp_mesh_get_layer());
        } else {
            mesh_light_set(0);
        }
    }
    return ESP_OK;
}
