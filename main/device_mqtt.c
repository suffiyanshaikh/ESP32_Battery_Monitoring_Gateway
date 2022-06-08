 #include "device_mqtt.h"

extern char device[20];
static const char *TAG = "device_mqtt";


/**
 * @brief error logs
 * 
 * @param message //error
 * @param error_code //message codes
 */

void log_error_if_nonzero(const char *message, int error_code)
{
    if (error_code != 0) {
        ESP_LOGE(TAG, "Last error %s: 0x%x", message, error_code);
    }
}

void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    ESP_LOGD(TAG, "Event dispatched from event loop base=%s, event_id=%d", base, event_id);
    esp_mqtt_event_handle_t event = event_data;
    esp_mqtt_client_handle_t client = event->client;
    int msg_id;
    switch ((esp_mqtt_event_id_t)event_id) {
    case MQTT_EVENT_CONNECTED:
        ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
        msg_id = esp_mqtt_client_publish(client,mqtt_topics.logs_topic,device, 0, 0, 0);
        ESP_LOGI(TAG, "sent publish successful  , msg_id=%d", msg_id);
        ESP_LOGI(TAG, " publish successful, topic=%s,data=%s", mqtt_topics.logs_topic,device);


        msg_id = esp_mqtt_client_subscribe(client,mqtt_topics.cmd_topic, 0);
        ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);
        ESP_LOGI(TAG, "sent subscribe successful, topic=%s", mqtt_topics.cmd_topic);


        break;
    case MQTT_EVENT_DISCONNECTED:
        ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
        break;

    case MQTT_EVENT_SUBSCRIBED:
        
        break;
    case MQTT_EVENT_UNSUBSCRIBED:
        ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_PUBLISHED:
        break;
    case MQTT_EVENT_DATA:
        ESP_LOGI(TAG, "MQTT_EVENT_DATA");
        printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
        printf("DATA=%.*s\r\n", event->data_len, event->data);

        char *cmd_ptr=(char*)calloc(50,event->data_len);//allocate memory for received data
        if(cmd_ptr==NULL)
        {
            printf("Memory not allocated");
        }
        else
        {
            strncpy(cmd_ptr,event->data,event->data_len);
            ESP_LOGI(TAG,"server cmd:%s",cmd_ptr);
            cmd_parser(cmd_ptr);
        }



        break;
    case MQTT_EVENT_ERROR:
        ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
        if (event->error_handle->error_type == MQTT_ERROR_TYPE_TCP_TRANSPORT) {
            log_error_if_nonzero("reported from esp-tls", event->error_handle->esp_tls_last_esp_err);
            log_error_if_nonzero("reported from tls stack", event->error_handle->esp_tls_stack_err);
            log_error_if_nonzero("captured as transport's socket errno",  event->error_handle->esp_transport_sock_errno);
            ESP_LOGI(TAG, "Last errno string (%s)", strerror(event->error_handle->esp_transport_sock_errno));

        }
        break;
    default:
        ESP_LOGI(TAG, "Other event id:%d", event->event_id);
        break;
    }
}

void mqtt_app_start(char mac_id[])
{

    
    sprintf(mqtt_topics.data_topic,"%s/data",mac_id);
    sprintf(mqtt_topics.cmd_topic,"%s/cmd",mac_id);
    sprintf(mqtt_topics.logs_topic,"%s/logs",mac_id);

    ESP_LOGI(TAG,"data_topic:\t%s\n",mqtt_topics.data_topic);
    ESP_LOGI(TAG,"cmd_topic:\t%s\n",mqtt_topics.cmd_topic);
    ESP_LOGI(TAG,"logs_topic:\t%s\n",mqtt_topics.logs_topic);



    esp_mqtt_client_config_t mqtt_config = {
    .uri = DEVICE_MQTT_URL,
    .username = DEVICE_MQTT_USERNAME,
    .password = DEVICE_MQTT_PASSWORD,
};
    
    esp_mqtt_client_handle_t client = esp_mqtt_client_init(&mqtt_config);
    /* The last argument may be used to pass data to the event handler, in this example mqtt_event_handler */
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
    esp_mqtt_client_start(client);

}

void cmd_parser(char *server_cmd)
{

ESP_LOGI(TAG,"server_cmd:%s\n",server_cmd);
free(server_cmd);
server_cmd=NULL;

}