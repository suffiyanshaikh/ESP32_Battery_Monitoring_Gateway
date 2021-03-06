#include "device_wifi.h"
#include "device_mqtt.h"

static const char *TAG = "wifi";

extern char mac_id[20];
extern esp_mqtt_client_handle_t client;
extern bool Mqtt_conn_flag;


/* The event group allows multiple bits for each event, but we only care about two events:
 * - we are connected to the AP with an IP
 * - we failed to connect after the maximum amount of retries */

const int CONNECTED_GOT_IP = BIT0;
const int DISCONNECTED = BIT1;


void  event_handler( void * event_handler_arg,esp_event_base_t event_base,int32_t event_id,void * even_data)
{

    switch (event_id)
    {
    case WIFI_EVENT_STA_START:

    esp_wifi_connect();
    ESP_LOGI(TAG,"WIFI STA_START");

    break;

    case WIFI_EVENT_STA_CONNECTED:
    ESP_LOGI(TAG,"Wifi connected with SSID:%s",EXAMPLE_ESP_WIFI_SSID);

    break;

    case  WIFI_EVENT_STA_DISCONNECTED:
    if(Mqtt_conn_flag == true)
    mqtt_app_stop();
    esp_wifi_connect();
    ESP_LOGI(TAG,"Trying to connect with SSID:%s ",EXAMPLE_ESP_WIFI_SSID);
    xEventGroupSetBits(Wifi_events, DISCONNECTED);

    break;

    case IP_EVENT_STA_GOT_IP:
     mqtt_app_start(mac_id);
    

    break;

    default:
        break;
    }

}


void wifi_init_sta(void)
{
   Wifi_events = xEventGroupCreate();

    ESP_ERROR_CHECK(esp_netif_init());

    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));


   ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT,ESP_EVENT_ANY_ID,event_handler,NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT,IP_EVENT_STA_GOT_IP,event_handler,NULL));
                                                       
   

    wifi_config_t wifi_config = {
        .sta = {
            .ssid = EXAMPLE_ESP_WIFI_SSID,
            .password = EXAMPLE_ESP_WIFI_PASS,
            /* Setting a password implies station will connect to all security modes including WEP/WPA.
             * However these modes are deprecated and not advisable to be used. Incase your Access point
             * doesn't support WPA2, these mode can be enabled by commenting below line */
	     .threshold.authmode = WIFI_AUTH_WPA2_PSK,

            .pmf_cfg = {
                .capable = true,
                .required = false
            },
        },
    };
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA) );
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config) );
    ESP_ERROR_CHECK(esp_wifi_start() );

    ESP_LOGI(TAG, "wifi_init_sta finished.");

//     /* Waiting until either the connection is established (WIFI_CONNECTED_BIT) or connection failed for the maximum
//      * number of re-tries (WIFI_FAIL_BIT). The bits are set by event_handler() (see above) */
//     EventBits_t bits = xEventGroupWaitBits(s_wifi_event_group,
//             WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
//             pdFALSE,
//             pdFALSE,
//             portMAX_DELAY);

//     /* xEventGroupWaitBits() returns the bits before the call returned, hence we can test which event actually
//      * happened. */
//     if (bits & WIFI_CONNECTED_BIT) {
//         ESP_LOGI(TAG, "connected to ap SSID:%s password:%s",
//                  EXAMPLE_ESP_WIFI_SSID, EXAMPLE_ESP_WIFI_PASS);
//     } else if (bits & WIFI_FAIL_BIT) {
//         ESP_LOGI(TAG, "Failed to connect to SSID:%s, password:%s",
//                  EXAMPLE_ESP_WIFI_SSID, EXAMPLE_ESP_WIFI_PASS);
//     } else {
//         ESP_LOGE(TAG, "UNEXPECTED EVENT");
//     }

//     /* The event will not be processed after unregister */
//    // ESP_ERROR_CHECK(esp_event_handler_instance_unregister(IP_EVENT, IP_EVENT_STA_GOT_IP, instance_got_ip));
//    // ESP_ERROR_CHECK(esp_event_handler_instance_unregister(WIFI_EVENT, ESP_EVENT_ANY_ID, instance_any_id));
    
//    //vEventGroupDelete(s_wifi_event_group);
}