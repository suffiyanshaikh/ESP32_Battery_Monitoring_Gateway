#include <stdio.h>
#include <string.h>
#include "esp_system.h"
#include <stdlib.h>
#include "esp_log.h"
#include "sdkconfig.h"


#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_event.h"
#include "nvs_flash.h"

#include "device_wifi.h"
#include "device_mqtt.h"

#define FW_Version 1.0
char mac_id[20];
uint8_t base_mac_addr[6] = {0};
char device[100];//



static const char *TAG = "device";

void app_main(void)
{

  //Initialize NVS
  esp_err_t err = nvs_flash_init();
  if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    ESP_ERROR_CHECK(nvs_flash_erase());
    err = nvs_flash_init();
  }
  ESP_ERROR_CHECK(err);

  //Read mac address
  esp_efuse_mac_get_default(base_mac_addr);
  sprintf(mac_id,"%x:%x:%x:%x:%x:%x", base_mac_addr[0], base_mac_addr[1], base_mac_addr[2], base_mac_addr[3], base_mac_addr[4], base_mac_addr[5]);
  ESP_LOGI(TAG,"mac_id = %s \n",mac_id);
  sprintf(device,"{\"device\":\"%s\",\"FW_Ver\":%0.2f}",mac_id,FW_Version);


  //Start wifi
  ESP_LOGI(TAG, "ESP_WIFI_MODE_STA");
  wifi_init_sta();

  ESP_LOGI(TAG, "ESP_WIFI_MODE_STA");
  mqtt_app_start(mac_id);


}