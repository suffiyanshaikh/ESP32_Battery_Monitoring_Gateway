#ifndef DEVICE_MQTT_H
#define DEVICE_MQTT_H

#ifdef __cplusplus
extern "C" {
#endif


#include "lwip/sockets.h"
#include "lwip/dns.h"
#include "lwip/netdb.h"
#include "mqtt_client.h"


/**
 * @brief MQTT Configuration
 * 
 * 
 */
#define DEVICE_MQTT_URL ESP_MQTT_URL
#define DEVICE_MQTT_USERNAME ESP_MQTT_USERNAME
#define DEVICE_MQTT_PASSWORD    ESP_MQTT_PASSWORD


typedef struct mqtt_configure
{
    
char data_topic[50];
char cmd_topic[50];
char logs_topic[50];


}mqtt_configure;
mqtt_configure mqtt_topics;


/**
 * @brief mqtt errors
 * @param  message message
 * @param error_code error_codes
 * 
 */
void log_error_if_nonzero(const char *message, int error_code);

/**
 * @brief Event handler registered to receive MQTT events
 *
 *  This function is called by the MQTT client event loop.
 *
 * @param handler_args user data registered to the event.
 * @param base Event base for the handler(always MQTT Base in this example).
 * @param event_id The id for the received event.
 * @param event_data The data for the event, esp_mqtt_event_handle_t.
 */
void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data);

void mqtt_app_start(char mac_id[]);
void cmd_parser(char *server_cmd);




#ifdef __cplusplus
}
#endif
#endif // mqtt