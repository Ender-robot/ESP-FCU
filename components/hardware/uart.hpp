#ifndef UART_HPP
#define UART_HPP

#include "driver/uart.h"
#include <cstdint>
#include <cstdio>
#include <cstdarg>
#include <cstring>
#include <vector>

class Uart {
    public:
        // 构造函数
        Uart ( 
            uart_port_t uart_id, //外设UART号
            int tx_pin, // 发送引脚
            int rx_pin  // 接受引脚
        );

        // 析构函数
        ~Uart();

        // 初始化
        bool init();

        // 发
        bool write(const uint8_t* data, int data_len);
        bool write(float data); // 浮点

        // 类似原版printf的用法
        bool printf(const char* fmt, ...) __attribute__((format(printf, 2, 3)));

        // 读
        int read(void* dataBuff, size_t data_len, TickType_t timeout);
        
    private:
        uart_port_t m_uart_id;
        int m_tx_pin;
        int m_rx_pin;
        bool success;

};

#endif
