#ifndef ADC_HPP
#define ADC_HPP

#include "esp_adc/adc_oneshot.h"

class Adc {
    public:
        Adc(adc_unit_t adc_id, adc_channel_t ch);
        ~Adc();

        int read();

    private:
        bool success;
        adc_unit_t m_adc_id;
        adc_channel_t m_ch;
        adc_oneshot_unit_handle_t adc_handle; // 用于配置adc的结构体
};

#endif
