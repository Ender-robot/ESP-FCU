#ifndef UART_DATA_PACK_HPP
#define UART_DATA_PACK_HPP

#include "cstdint"

#define HEADER 0x55AA // 帧头

// 指令串口数据包
struct RoBoCmd
{
    uint16_t header = 0x00;
    float val1 = 0x00;
    float val2 = 0x00;
    float val3 = 0x00;
    float val4 = 0x00;
    uint8_t mode1 = 0x00;
    uint8_t mode2 = 0x00;
    uint16_t checksum = 0x00;    // CRC-16
}__attribute__((packed)); // 不进行字节对齐


// 反馈串口数据包
struct RoBoFeedBack
{
    uint16_t header = HEADER;
    float roll = 0.0f;
    float pitch = 0.0f;
    float yaw = 0.0f;
    float reserved1 = 0.0f;
    float altitudev = 0.0f;
    float vel_x = 0.0f;
    float vel_y = 0.0f;
    float vel_z = 0.0f;
    uint8_t  status = 0x00;
    uint8_t  reserved2 = 0x00;
    uint16_t checksum = 0x00;    // CRC-16
}__attribute__((packed)); // 不进行字节对齐

#endif
