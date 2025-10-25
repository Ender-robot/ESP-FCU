#include "system.hpp"

// freertos毫秒计时任务
void _time_task(int ms) { 
        vTaskDelay(pdMS_TO_TICKS(ms));
}

// 秒级延时
void delay(int sec) {
    int ms = sec * 1000;
    _time_task(ms); 
}

// 毫秒级延时
void delay_ms(int ms) {
    _time_task(ms);
}

/**
 * @brief 速度控制类，类似ROS的用法
 * 
 * @param hz 频率
 */
Rate::Rate(float hz) 
    : m_hz(hz), xLastWakeTime(xTaskGetTickCount()), tick(pdMS_TO_TICKS(FREEROTS_RATE / hz)){
}

/**
 * @brief 自动更新时间戳，休眠直到时间达标
 */
void Rate::sleep() {
    vTaskDelayUntil(&xLastWakeTime, tick);
}

/**
 * @brief 获取设置频率
 */
float Rate::getRate() const {
    return m_hz;
}

// uint8转int16
int16_t UINT8_2_INT16(uint8_t highByte, uint8_t lowByte) {
     /* 1. 先将高字节左移 8 位，并与低字节进行位或操作，得到一个无符号的 16 位值
        这里要确保 highByte 和 lowByte 都先提升到至少 uint16_t，避免移位截断 */
    uint16_t raw_uint16 = (static_cast<uint16_t>(highByte) << 8) | static_cast<uint16_t>(lowByte);

    /* 2. 将无符号的 16 位值强制转换为有符号的 16 位值
    C++ 会正确处理二进制补码表示的负数转换 */
    return static_cast<int16_t>(raw_uint16);
}
