#include "pwm.hpp"

Pwm::Pwm(ledc_timer_t tim_id, ledc_channel_t ch, int pin, uint32_t freq) 
    : success(false), m_tim_id(tim_id), m_ch(ch), m_pin(pin), m_freq(freq) {
    // 创建定时器配置结构体
    ledc_timer_config_t ledc_timer = {
        .speed_mode       = LEDC_HIGH_SPEED_MODE,
        .duty_resolution  = LEDC_TIMER_16_BIT,
        .timer_num        = m_tim_id,
        .freq_hz          = m_freq,
        .clk_cfg          = LEDC_AUTO_CLK, // 自动选择最佳时钟源
    };
    // 创建LEDC通道配置结构体
    ledc_channel_config_t ledc_channel = {
        .gpio_num         = m_pin,
        .speed_mode       = LEDC_HIGH_SPEED_MODE,
        .channel          = m_ch,
        .timer_sel        = m_tim_id,
        .duty             = 0, // 初始占空比为0
        .hpoint           = 0, // 通常设为0，表示占空比的起始点
    };
    // 设置定时器
    esp_err_t err;
    err = ledc_timer_config(&ledc_timer);
    if (err != ESP_OK)
        return;
    err = ledc_channel_config(&ledc_channel);
    if (err != ESP_OK)
        return;
    err = ledc_fade_func_install(0);
    if (err != ESP_OK)
        return;

    success = true;
}

Pwm::~Pwm() {
    if (success)
        ledc_stop(LEDC_HIGH_SPEED_MODE, m_ch, 0);
}

bool Pwm::pwm_output_duty(uint32_t duty) {
    if (success) {
        esp_err_t err;
        err = ledc_set_duty_and_update(LEDC_HIGH_SPEED_MODE, m_ch, duty, 0);
        if (err != ESP_OK)
            return false;
        else
            return true;
    }
    else
        return false;
}

bool Pwm::pwm_output_us(uint32_t us) {
    uint32_t duty = us * m_freq / 1000000.0 * 65535;
    if (success) {
        esp_err_t err;
        err = ledc_set_duty_and_update(LEDC_HIGH_SPEED_MODE, m_ch, duty, 0);
        if (err != ESP_OK)
            return false;
        else
            return true;
    }
    else
        return false;
}
