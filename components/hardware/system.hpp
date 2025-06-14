#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// freertos毫秒计时任务
void _time_task(int ms);
// 秒级延时
void delay(int sec);
// 毫秒级延时
void delay_ms(int ms);
// uint8转int16
int16_t UINT8_2_INT16(uint8_t highByte, uint8_t lowByte);

#endif
