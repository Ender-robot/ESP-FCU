#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define FREEROTS_RATE 1000 // freerots频率

// freertos毫秒计时任务
void _time_task(int ms);
// 秒级延时
void delay(int sec);
// 毫秒级延时
void delay_ms(int ms);

/**
 * @brief 速度控制类，类似ROS的用法
 * 
 * @param hz 频率
 */
class Rate {
    public:
        Rate(float hz);

        void sleep();
        float getRate() const;
    private:
        float m_hz;
        TickType_t xLastWakeTime;
        int tick;
};

// uint8转int16
int16_t UINT8_2_INT16(uint8_t highByte, uint8_t lowByte);

#endif
