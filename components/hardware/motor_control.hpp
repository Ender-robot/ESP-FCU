#ifndef MOTOR_CONTROL_HPP
#define MOTOR_CONTROL_HPP

#include "pwm.hpp"
#include "system.hpp"

// 无刷电机控制类
class Motor {
    public:
        Motor(ledc_timer_t tim_id, ledc_channel_t ch, int pin, uint32_t freq, uint32_t max = 2000, uint32_t min = 1000);
        ~Motor();

        void cal_pwm(); // pwm油门行程校准

        void arm(); // 武装
        void dearm(); // 解除武装

        void output(uint32_t us); // 输出pwm信号

    private:
        bool success;
        ledc_timer_t m_tim_id;
        ledc_channel_t m_ch;
        int m_pin;
        uint32_t m_freq;
        uint32_t m_max; 
        uint32_t m_min;

        Pwm pwm_output;
};

#endif
