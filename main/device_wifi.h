#ifndef DEVICE_WIFI_H
#define DEVICE_WIFI_H

#ifdef __cplusplus
extern "C" {
#endif


#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"

#include "lwip/err.h"
#include "lwip/sys.h"

/* The examples use WiFi configuration that you can set via project configuration menu

   If you'd rather not, just change the below entries to strings with
   the config you want - ie #define EXAMPLE_WIFI_SSID "mywifissid"
*/
#define EXAMPLE_ESP_WIFI_SSID      CONFIG_ESP_WIFI_SSID
#define EXAMPLE_ESP_WIFI_PASS      CONFIG_ESP_WIFI_PASSWORD
#define EXAMPLE_ESP_MAXIMUM_RETRY  CONFIG_ESP_MAXIMUM_RETRY

wifi_ap_record_t wifidata; //To read wifi data
int8_t rssi;               //wifi rssi



/* FreeRTOS event group to signal when we are connected*/
 EventGroupHandle_t Wifi_events;


void event_handler(void* arg, esp_event_base_t event_base,
                                int32_t event_id, void* event_data);

/**
 * @brief Intialize wifi in station mode
 * 
 */
void wifi_init_sta(void);


#ifdef __cplusplus
}
#endif
#endif // wifi