#include "datapack.hpp"

COMM::COMM(Uart& uartClass) :
    m_uart(uartClass), packStatus(WAITING_FOR_HEADER_1), rx_cnt(0) {
}

COMM::~COMM() {
}

// CRC校验表
const uint16_t COMM::crc16Table[256] = {
    0x0000, 0xC0C1, 0xC181, 0x0140, 0xC301, 0x03C0, 0x0280, 0xC241, 
    0xC601, 0x06C0, 0x0780, 0xC741, 0x0500, 0xC5C1, 0xC481, 0x0440, 
    0xCC01, 0x0CC0, 0x0D80, 0xCD41, 0x0F00, 0xCFC1, 0xCE81, 0x0E40, 
    0x0A00, 0xCAC1, 0xCB81, 0x0B40, 0xC901, 0x09C0, 0x0880, 0xC841, 
    0xD801, 0x18C0, 0x1980, 0xD941, 0x1B00, 0xDBC1, 0xDA81, 0x1A40, 
    0x1E00, 0xDEC1, 0xDF81, 0x1F40, 0xDD01, 0x1DC0, 0x1C80, 0xDC41, 
    0x1400, 0xD4C1, 0xD581, 0x1540, 0xD701, 0x17C0, 0x1680, 0xD641, 
    0xD201, 0x12C0, 0x1380, 0xD341, 0x1100, 0xD1C1, 0xD081, 0x1040, 
    0xF001, 0x30C0, 0x3180, 0xF141, 0x3300, 0xF3C1, 0xF281, 0x3240, 
    0x3600, 0xF6C1, 0xF781, 0x3740, 0xF501, 0x35C0, 0x3480, 0xF441, 
    0x3C00, 0xFCC1, 0xFD81, 0x3D40, 0xFF01, 0x3FC0, 0x3E80, 0xFE41, 
    0xFA01, 0x3AC0, 0x3B80, 0xFB41, 0x3900, 0xF9C1, 0xF881, 0x3840, 
    0x2800, 0xE8C1, 0xE981, 0x2940, 0xEB01, 0x2BC0, 0x2A80, 0xEA41, 
    0xEE01, 0x2EC0, 0x2F80, 0xEF41, 0x2D00, 0xEDC1, 0xEC81, 0x2C40, 
    0xE401, 0x24C0, 0x2580, 0xE541, 0x2700, 0xE7C1, 0xE681, 0x2640, 
    0x2200, 0xE2C1, 0xE381, 0x2340, 0xE101, 0x21C0, 0x2080, 0xE041, 
    0xA001, 0x60C0, 0x6180, 0xA141, 0x6300, 0xA3C1, 0xA281, 0x6240, 
    0x6600, 0xA6C1, 0xA781, 0x6740, 0xA501, 0x65C0, 0x6480, 0xA441, 
    0x6C00, 0xACC1, 0xAD81, 0x6D40, 0xAF01, 0x6FC0, 0x6E80, 0xAE41, 
    0xAA01, 0x6AC0, 0x6B80, 0xAB41, 0x6900, 0xA9C1, 0xA881, 0x6840, 
    0x7800, 0xB8C1, 0xB981, 0x7940, 0xBB01, 0x7BC0, 0x7A80, 0xBA41, 
    0xBE01, 0x7EC0, 0x7F80, 0xBF41, 0x7D00, 0xBDC1, 0xBC81, 0x7C40, 
    0xB401, 0x74C0, 0x7580, 0xB541, 0x7700, 0xB7C1, 0xB681, 0x7640, 
    0x7200, 0xB2C1, 0xB381, 0x7340, 0xB101, 0x71C0, 0x7080, 0xB041, 
    0x5000, 0x90C1, 0x9181, 0x5140, 0x9301, 0x53C0, 0x5280, 0x9241, 
    0x9601, 0x56C0, 0x5780, 0x9741, 0x5500, 0x95C1, 0x9481, 0x5440, 
    0x9C01, 0x5CC0, 0x5D80, 0x9D41, 0x5F00, 0x9FC1, 0x9E81, 0x5E40, 
    0x5A00, 0x9AC1, 0x9B81, 0x5B40, 0x9901, 0x59C0, 0x5880, 0x9841, 
    0x8801, 0x48C0, 0x4980, 0x8941, 0x4B00, 0x8BC1, 0x8A81, 0x4A40, 
    0x4E00, 0x8EC1, 0x8F81, 0x4F40, 0x8D01, 0x4DC0, 0x4C80, 0x8C41, 
    0x4400, 0x84C1, 0x8581, 0x4540, 0x8701, 0x47C0, 0x4680, 0x8641, 
    0x8201, 0x42C0, 0x4380, 0x8341, 0x4100, 0x81C1, 0x8081, 0x4040
};

/**
 * @brief 生成16位CRC校验和，CRC-16/MODBUS 标准
 * 
 * @param bytesBuf 需要生成校验的实际发送的字节流
 * @param len 除校验和以外长度
 * 
 * @return 校验和
 */
uint16_t COMM::_createCRC16(const void* bytesBuf, size_t len) const {
    uint16_t crc = 0xFFFF;
    const uint8_t* data = static_cast<const uint8_t*>(bytesBuf);

    for (size_t i = 0; i < len; ++i) {
        uint8_t index = (crc ^ data[i]) & 0xFF;
        crc = (crc >> 8) ^ crc16Table[index];
    }
    return crc;
}

/**
 * @brief 以串口发送数据包
 * 
 * @param pack 要发送的数据包，传入时无需自己添加检验和，函数内自动生成
 */
bool COMM::uartSendPack(RoBoFeedBack& pack) {
    size_t len = sizeof(pack); // 检查包长度
    uint16_t crc = _createCRC16(&pack, len - 2); // 生成CRC16，长度减2是因为最后两个字节是16位CRC校验和
    pack.checksum = crc; // 传入校验和
    return m_uart.write(&pack, len); // 发送
}

/**
 * @brief 从串口收取一次数据，并尝试解包数据包，单次未解出完整的数据包会暂存进传入对象的缓冲区
 * 
 * @param packBuf 解包成功数据包的缓存区
 * 
 * @return 成功解出完整的包返回true，此时只要读取传入的缓冲区即可
 */
bool COMM::uartRecePack(RoBoCmd& packBuf) {
    // 1. 从串口读取所有可用的数据到临时缓冲区
    // 注意：这里要用 m_rx_temp_buf，而不是旧的 bytesBuf
    int len = m_uart.read(m_rx_temp_buf, sizeof(m_rx_temp_buf), 0); // 非阻塞读取

    if (len <= 0) {
        return false; // 没有新数据，直接返回
    }
    
    // 2. 逐字节将新数据送入状态机处理
    for (int i = 0; i < len; ++i) {
        uint8_t byte = m_rx_temp_buf[i];

        switch (packStatus) {
            case WAITING_FOR_HEADER_1:
                if (byte == 0x55) {
                    // 找到了第一个帧头，将其存入组包缓冲区的起始位置
                    m_rx_packet_buf[0] = byte;
                    rx_cnt = 1; // 已经收到了1个字节
                    packStatus = WAITING_FOR_HEADER_2;
                }
                break;

            case WAITING_FOR_HEADER_2:
                if (byte == 0xAA) {
                    // 找到了第二个帧头，存入组包缓冲区
                    m_rx_packet_buf[1] = byte;
                    rx_cnt = 2; // 已经收到了2个字节
                    packStatus = RECEIVING_DATA;
                } else {
                    // 序列错误，重置状态机
                    // 如果当前字节恰好是新的帧头1，则可以直接进入 HEADER_2 状态
                    if (byte == 0x55) {
                        m_rx_packet_buf[0] = byte;
                        rx_cnt = 1;
                        // 状态保持 WAITING_FOR_HEADER_2
                    } else {
                        packStatus = WAITING_FOR_HEADER_1;
                    }
                }
                break;

            case RECEIVING_DATA:
                // 将数据负载和校验和存入组包缓冲区
                m_rx_packet_buf[rx_cnt++] = byte;

                // 3. 检查是否已收到一个完整的数据包
                if (rx_cnt == sizeof(RoBoCmd)) { // RoBoCmd 总长为22字节
                    // 定义数据包长度常量，提高可读性
                    constexpr size_t full_packet_len = sizeof(RoBoCmd); // 22
                    constexpr size_t checksum_data_len = full_packet_len - 2; // 20

                    // 4. 执行CRC校验 (根据你的要求，CRC包含帧头)
                    // 计算范围是 m_rx_packet_buf 中的前20个字节
                    uint16_t calculated_crc = _createCRC16(m_rx_packet_buf, checksum_data_len);

                    // 从接收到的数据中提取校验和 (最后两个字节)
                    uint16_t received_checksum = *reinterpret_cast<uint16_t*>(&m_rx_packet_buf[checksum_data_len]);
                    
                    // 无论校验成功与否，都重置状态机，准备接收下一个包
                    packStatus = WAITING_FOR_HEADER_1;
                    rx_cnt = 0;

                    // ESP_LOGI("RxPack", "%X  %X", calculated_crc, received_checksum);
                    if (calculated_crc == received_checksum) {
                        // 校验成功！将组包缓冲区的内容完整地拷贝到输出参数
                        memcpy(&packBuf, m_rx_packet_buf, full_packet_len);
                        return true; // 成功解析一个包
                    }
                    // 如果校验失败，不做任何事，函数将会在 for 循环结束后返回 false
                    // 状态机已经重置，可以继续在当前批次剩余的字节中寻找新包头
                }
                break;
        } // end switch
    } // end for

    // 遍历完所有新数据，但尚未形成一个完整的包
    return false;
}
