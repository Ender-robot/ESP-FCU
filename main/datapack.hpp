#ifndef DATAPACK_HPP
#define DATAPACK_HPP

#include "uart.hpp"
#include "uart_data_pack.hpp"

/**
 * @brief 通讯类，负责通过串口发送反馈和通过串口接受指令
 * 
 * @param uartClass 使用的串口对象
 */
class COMM {
    public:
        COMM(Uart& uartClass);
        ~COMM();

        bool uartSendPack(RoBoFeedBack& pack);
    private:
        Uart& m_uart; // 传入的串口对象
        static const uint16_t crc16Table[256]; // CRC-16/MODBUS查找表(多项式 0x8005)

        uint16_t _createCRC16(const void* bytesBuf, size_t len) const; //计算16位CRC
};

#endif
