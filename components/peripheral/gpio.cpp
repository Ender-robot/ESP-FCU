#include "gpio.hpp"

/**
 * @brief 创建GPIO类
 */
GPIO::GPIO(
    gpio_num_t num, 
    gpio_mode_t mode,
    uint8_t init_pin,
    pull_mode pull_mode, 
    gpio_int_type_t int_type
) 
    : success(false), m_num(num), m_mode(mode), m_init_pin(init_pin), m_pull_mode(pull_mode), m_int_type(int_type) {

    if (m_pull_mode == GPIO_DOWN) {
        up_en = GPIO_PULLUP_DISABLE;
        down_en = GPIO_PULLDOWN_ENABLE;
    }
    else if (m_pull_mode == GPIO_UP) {
        up_en = GPIO_PULLUP_ENABLE;
        down_en = GPIO_PULLDOWN_DISABLE;
    }
    else if (m_pull_mode == GPIO_DIS) {
        up_en = GPIO_PULLUP_DISABLE;
        down_en = GPIO_PULLDOWN_DISABLE;
    }

    gpio_mask = (1ULL << m_num);
}

GPIO::~GPIO() {

}

/**
 * @brief 初始化GPIO
 */
bool GPIO::init() {
    gpio_config_t config = {
        .pin_bit_mask = gpio_mask,
        .mode = m_mode,
        .pull_up_en = up_en,
        .pull_down_en = down_en,
        .intr_type = m_int_type
    };

    esp_err_t err = gpio_config(&config);

    if (err == ESP_OK) 
        if (m_mode == GPIO_MODE_OUTPUT) gpio_set_level(m_num, m_init_pin);

    if (err == ESP_OK) {
        success = true;
        return true;
    }
    else return false;
}

/**
 * @brief 设置gpio的输出
 */
bool GPIO::write(uint8_t pin) {
    if (!success || m_mode == GPIO_MODE_INPUT) {
        return false;
    }

    gpio_set_level(m_num, pin);

    return true;
}
