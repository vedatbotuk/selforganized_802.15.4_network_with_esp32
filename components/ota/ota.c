#include "esp_ota_ops.h"
#include "esp_timer.h"
#include "esp_log.h"
#include "esp_check.h"
#include "esp_zigbee_core.h"

static const char *TAG_OTA = "OTA_UPDATE";

static const esp_partition_t *s_ota_partition = NULL;
static esp_ota_handle_t s_ota_handle = 0;
size_t ota_data_len_ = 0;
uint8_t* ota_header_ = NULL;
size_t ota_header_size_ = 0;
bool ota_upgrade_subelement_ = false;

size_t min_size_t(size_t a, size_t b) {
    return (a < b) ? a : b;
}

void clear_ota_header() {
    // Free the allocated memory
    free(ota_header_);
    
    // Set size to 0
    ota_header_size_ = 0;
    
    // Set the pointer to NULL to avoid accessing freed memory accidentally
    ota_header_ = NULL;
}

esp_err_t zb_ota_upgrade_status_handler(esp_zb_zcl_ota_upgrade_value_message_t messsage)
{
    static uint32_t total_size = 0;
    static uint32_t offset = 0;
    static int64_t start_time = 0;
    const uint8_t *payload = (const uint8_t *)messsage.payload;
    size_t payload_size = messsage.payload_size;
    
    esp_err_t ret = ESP_OK;
    if (messsage.info.status == ESP_ZB_ZCL_STATUS_SUCCESS) {
        switch (messsage.upgrade_status) {
        case ESP_ZB_ZCL_OTA_UPGRADE_STATUS_START:
            ESP_LOGI(TAG_OTA, "-- OTA upgrade start");
            start_time = esp_timer_get_time();
            s_ota_partition = esp_ota_get_next_update_partition(NULL);
            assert(s_ota_partition);
            ret = esp_ota_begin(s_ota_partition, OTA_WITH_SEQUENTIAL_WRITES, &s_ota_handle);
            clear_ota_header();
            ota_upgrade_subelement_ = false;
            ota_data_len_ = 0;
            ESP_RETURN_ON_ERROR(ret, TAG_OTA, "Failed to begin OTA partition, status: %s", esp_err_to_name(ret));
            break;
        case ESP_ZB_ZCL_OTA_UPGRADE_STATUS_RECEIVE:
            total_size = messsage.ota_header.image_size;
            offset += messsage.payload_size;
            
            ESP_LOGI(TAG_OTA, "OTA [%ld/%ld]", offset, total_size);

            while (ota_header_size_ < 6 && payload_size > 0) {
                ota_header_ = realloc(ota_header_, (ota_header_size_ + 1) * sizeof(uint8_t));
				ota_header_[ota_header_size_] = payload[0];
				payload++;
				payload_size--;
				ota_header_size_++;
			}
            if (!ota_upgrade_subelement_ && ota_header_size_ == 6) {
                if (ota_header_[0] == 0 && ota_header_[1] == 0) {
                    ota_upgrade_subelement_ = true;
                    ota_data_len_ =   (((int)ota_header_[5] & 0xFF) << 24)
                                    | (((int)ota_header_[4] & 0xFF) << 16)
                                    | (((int)ota_header_[3] & 0xFF) << 8 )
                                    |  ((int)ota_header_[2] & 0xFF);                 
                    ESP_LOGI(TAG_OTA, "in if OTA sub-element size %zu", ota_data_len_);
                }
            }  
            if (ota_data_len_) {
                payload_size = min_size_t(ota_data_len_, payload_size);
                ota_data_len_ = ota_data_len_ - payload_size;

                ret = esp_ota_write(s_ota_handle, payload , payload_size);
                ESP_RETURN_ON_ERROR(ret, TAG_OTA, "Failed to write OTA data to partition, status: %s", esp_err_to_name(ret));   
            }
            break;
        case ESP_ZB_ZCL_OTA_UPGRADE_STATUS_APPLY:
            ESP_LOGI(TAG_OTA, "-- OTA upgrade apply");
            break;
        case ESP_ZB_ZCL_OTA_UPGRADE_STATUS_CHECK:
            ret = offset == total_size ? ESP_OK : ESP_FAIL;
            ESP_LOGI(TAG_OTA, "-- OTA upgrade check status: %s", esp_err_to_name(ret));
            break;
        case ESP_ZB_ZCL_OTA_UPGRADE_STATUS_FINISH:
            ESP_LOGI(TAG_OTA, "-- OTA Finish");
            ESP_LOGI(TAG_OTA,
                     "-- OTA Information: version: 0x%lx, manufactor code: 0x%x, image type: 0x%x, total size: %ld bytes, cost time: %lld ms,",
                     messsage.ota_header.file_version, messsage.ota_header.manufacturer_code, messsage.ota_header.image_type,
                     messsage.ota_header.image_size, (esp_timer_get_time() - start_time) / 1000);
            ret = esp_ota_end(s_ota_handle);
            ESP_RETURN_ON_ERROR(ret, TAG_OTA, "Failed to end OTA partition, status: %s", esp_err_to_name(ret));
            ret = esp_ota_set_boot_partition(s_ota_partition);
            ESP_RETURN_ON_ERROR(ret, TAG_OTA, "Failed to set OTA boot partition, status: %s", esp_err_to_name(ret));
            ESP_LOGW(TAG_OTA, "Prepare to restart system");
            esp_restart();
            break;
        default:
            ESP_LOGI(TAG_OTA, "OTA status: %d", messsage.upgrade_status);
            break;
        }
    }
    return ret;
}
