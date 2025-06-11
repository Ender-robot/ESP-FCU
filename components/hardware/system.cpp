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
