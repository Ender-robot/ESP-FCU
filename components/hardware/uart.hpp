#ifndef UART_HPP
#define UART_HPP

#include "driver/uart.h"
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

        // 发
        bool write(const uint8_t* data, int data_len);
        bool write(float data); // 浮点

        // 以ASCII的形式发送str
        bool writeAsASCII(char* str);
        // 以ASCII的形式发float
        bool writeAsASCII(float data);

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
        bool success;

};

#endif
