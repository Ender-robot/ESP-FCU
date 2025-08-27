#include "motor_control.hpp"

Motor::Motor(ledc_timer_t tim_id, ledc_channel_t ch, int pin, uint32_t freq, uint32_t max, uint32_t min) 
    : success(false), m_tim_id(tim_id), m_ch(ch), m_pin(pin), m_freq(freq),
      m_max(max), m_min(min), pwm_output(m_tim_id, m_ch, m_pin, m_freq) {

    pwm_output.pwm_output_duty(0);
}

Motor::~Motor() {

}

void Motor::cal_pwm() {
    pwm_output.pwm_output_us(m_max);
    delay(1);
    pwm_output.pwm_output_us(m_min);
    delay_ms(800);
    pwm_output.pwm_output_us(0);
}

void Motor::arm() {
  pwm_output.pwm_output_us(m_min);
}

void Motor::dearm() {
  pwm_output.pwm_output_us(m_min);
  delay_ms(200);
  pwm_output.pwm_output_us(0);
}

void Motor::output(uint32_t us) {
  pwm_output.pwm_output_us(us);
}
