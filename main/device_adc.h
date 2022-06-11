#ifndef DEVICE_ADC_H
#define DEVICE_ADC_H

#ifdef __cplusplus
extern "C" {
#endif


#include "driver/adc.h"
#include "esp_adc_cal.h"



void check_efuse(void);

void print_char_val_type(esp_adc_cal_value_t val_type);

void adc_init();

void get_voltage();



#ifdef __cplusplus
}
#endif
#endif // mqtt