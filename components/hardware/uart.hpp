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
        bool read(int data_len);
        uint8_t data[128]; // 数据缓冲区

        // 数据包结构体
        struct data_pack
        {
            uint8_t mode;
            float x;
            float y;
            float z;
        };
        // 标志字节
        const uint8_t START_BYTE = 0x7E;
        const uint8_t   END_BYTE = 0x7E;
        const uint8_t ESCAPE_BYTE = 0x7D;
        const uint8_t ESCAPE_XOR_MASK = 0x20;
        // 发数据包(私有协议)
        bool write_data_pack(uint8_t mode, float x, float y, float z);
        // 读数据包(私有协议)
        data_pack read_data_pack();
        
    private:
        uart_port_t m_uart_id;
        int m_tx_pin;
        int m_rx_pin;
        bool success;

};

#endif
