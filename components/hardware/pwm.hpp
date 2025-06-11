#ifndef PWM_HPP
#define PWM_HPP

#include "driver/ledc.h"

class Pwm {
    public:
        Pwm(ledc_timer_t tim_id, ledc_channel_t ch, int pin, uint32_t freq);
        ~Pwm();

        bool pwm_output_duty(uint32_t duty);
        bool pwm_output_us(uint32_t us);

    private:
        bool success;
        ledc_timer_t m_tim_id;
        ledc_channel_t m_ch;
        int m_pin;
        uint32_t m_freq;
};

#endif
