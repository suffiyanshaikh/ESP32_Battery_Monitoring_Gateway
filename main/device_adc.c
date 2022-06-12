
#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "device_adc.h"
#include "device_mqtt.h"
#include "device_wifi.h"

static const char *TAG = "ADC";


extern esp_mqtt_client_handle_t client;
extern char mac_id[20];
char Json_String[100];      //Json
extern struct mqtt_configure mqtt_topics;
extern int8_t rssi;
extern wifi_ap_record_t wifidata;




#define DEFAULT_VREF    1100        //Use adc2_vref_to_gpio() to obtain a better estimate
#define NO_OF_SAMPLES   64          //Multisampling

esp_adc_cal_characteristics_t *adc_chars;
#if CONFIG_IDF_TARGET_ESP32
const adc_channel_t channel = ADC_CHANNEL_0;     //GPIO34 if ADC1
const adc_bits_width_t width = ADC_WIDTH_BIT_12;
#elif CONFIG_IDF_TARGET_ESP32S2
const adc_channel_t channel = ADC_CHANNEL_0;     // GPIO7 if ADC1,
const adc_bits_width_t width = ADC_WIDTH_BIT_13;
#endif
const adc_atten_t atten = ADC_ATTEN_DB_11;
const adc_unit_t unit = ADC_UNIT_1;


void check_efuse(void)
{
#if CONFIG_IDF_TARGET_ESP32
    //Check if TP is burned into eFuse
    if (esp_adc_cal_check_efuse(ESP_ADC_CAL_VAL_EFUSE_TP) == ESP_OK) {
        printf("eFuse Two Point: Supported\n");
    } else {
        printf("eFuse Two Point: NOT supported\n");
    }
    //Check Vref is burned into eFuse
    if (esp_adc_cal_check_efuse(ESP_ADC_CAL_VAL_EFUSE_VREF) == ESP_OK) {
        printf("eFuse Vref: Supported\n");
    } else {
        printf("eFuse Vref: NOT supported\n");
    }
#elif CONFIG_IDF_TARGET_ESP32S2
    if (esp_adc_cal_check_efuse(ESP_ADC_CAL_VAL_EFUSE_TP) == ESP_OK) {
        printf("eFuse Two Point: Supported\n");
    } else {
        printf("Cannot retrieve eFuse Two Point calibration values. Default calibration values will be used.\n");
    }
#else
#error "This example is configured for ESP32/ESP32S2."
#endif
}

void print_char_val_type(esp_adc_cal_value_t val_type)
{
    if (val_type == ESP_ADC_CAL_VAL_EFUSE_TP) {
        printf("Characterized using Two Point Value\n");
    } else if (val_type == ESP_ADC_CAL_VAL_EFUSE_VREF) {
        printf("Characterized using eFuse Vref\n");
    } else {
        printf("Characterized using Default Vref\n");
    }
}

void adc_init()
{

//Check if Two Point or Vref are burned into eFuse
check_efuse();

//Configure ADC
if (unit == ADC_UNIT_1) {
    adc1_config_width(width);
    adc1_config_channel_atten(channel, atten);
} 

//Characterize ADC
adc_chars = calloc(1, sizeof(esp_adc_cal_characteristics_t));
esp_adc_cal_value_t val_type = esp_adc_cal_characterize(unit, atten, width, DEFAULT_VREF, adc_chars);
print_char_val_type(val_type);

}

void get_voltage()
{

    uint32_t adc_reading = 0;
        //Multisampling
        for (int i = 0; i < NO_OF_SAMPLES; i++) {
            if (unit == ADC_UNIT_1) {
                adc_reading += adc1_get_raw((adc1_channel_t)channel);
            } 
        }
        adc_reading /= NO_OF_SAMPLES;
        //Convert adc_reading to voltage in mV
        uint32_t voltage = esp_adc_cal_raw_to_voltage(adc_reading, adc_chars);
        printf("Raw: %d\tVoltage: %dmV\n", adc_reading, voltage);
        vTaskDelay(pdMS_TO_TICKS(1000));

        esp_wifi_sta_get_ap_info(&wifidata);
        rssi= wifidata.rssi;


        sprintf(Json_String,"{\"device\":\"%s\",\"rssi\":\"%d\",\"data\":[\"Volt\":\"%0.2f\"]}", mac_id,rssi,voltage*0.001);
        int  msg_id = esp_mqtt_client_publish(client,mqtt_topics.data_topic,Json_String, 0, 0, 1);
        ESP_LOGI(TAG, "sent publish successful,msg_id=%d", msg_id);
        ESP_LOGI(TAG, " publish successfull on topic=%s", mqtt_topics.data_topic);
        ESP_LOGI(TAG, " publish successfull data=%s",Json_String);
        printf("\n");


    }







