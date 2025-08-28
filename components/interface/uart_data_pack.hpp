#ifndef UART_DATA_PACK_HPP
#define UART_DATA_PACK_HPP

#include "cstdint"

#define HEADER 0x55AA // 帧头

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
    uint16_t checksum;    // CRC-16
}__attribute__((packed)); // 不进行字节对齐

#endif
