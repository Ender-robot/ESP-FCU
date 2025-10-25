#include "adc.hpp"

Adc::Adc(adc_unit_t adc_id, adc_channel_t ch) 
    : success(false), m_adc_id(adc_id), m_ch(ch) {
        esp_err_t err;
    // 配置ADC模数转换器
    adc_oneshot_unit_init_cfg_t init_config = {
        .unit_id = m_adc_id,
    };
    err = adc_oneshot_new_unit(&init_config, &adc_handle);
    if (err != ESP_OK)
        return;
    // 配置 ADC 通道
    adc_oneshot_chan_cfg_t config = {
        .atten = ADC_ATTEN_DB_12,
        .bitwidth = ADC_BITWIDTH_DEFAULT
    };
    err = adc_oneshot_config_channel(adc_handle, m_ch, &config);
    if (err != ESP_OK)
        return;
    success = true;
}

Adc::~Adc() {
    if (success)
        adc_oneshot_del_unit(adc_handle);
}

int Adc::read() {
    int val;
    if (success)
        adc_oneshot_read(adc_handle, m_ch, &val);
    else
        val = -1;
    return val;
}
