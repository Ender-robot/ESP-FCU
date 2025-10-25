#ifndef GPIO_HPP
#define GPIO_HPP

#include "driver/gpio.h"

/**
 * @brief 用于控制GPIO的电阻模式
 */
enum pull_mode {
    GPIO_DOWN,
    GPIO_UP,
    GPIO_DIS
};

/**
 * @brief 用控制GPIO高低电平输入输出的类
 * 
*/
class GPIO {
    public:
        GPIO(
            gpio_num_t num, 
            gpio_mode_t mode, 
            uint8_t init_pin = 0,
            pull_mode pull_mode = GPIO_DIS, 
            gpio_int_type_t int_type = GPIO_INTR_DISABLE
        );

        ~GPIO();

        bool init();

        bool write(uint8_t pin);

    private:
        bool success;
        gpio_num_t m_num;
        gpio_mode_t m_mode;
        uint8_t m_init_pin;
        pull_mode m_pull_mode;
        gpio_int_type_t m_int_type;

        gpio_pullup_t up_en;
        gpio_pulldown_t down_en;
        uint64_t gpio_mask;
};

#endif
