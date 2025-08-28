#include "uart.hpp"

// 构造函数
Uart::Uart(uart_port_t uart_id, int tx_pin, int rx_pin)
    : m_uart_id(uart_id), m_tx_pin(tx_pin), m_rx_pin(rx_pin), success(false) {
}

// 析构函数
Uart::~Uart() {
    if (success)
        uart_driver_delete(m_uart_id); // 删除uart硬件驱动
}

// 初始化
bool Uart::init() {
    // 将初始化参数传入结构体
    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .rx_flow_ctrl_thresh = 0,
    };
    // 设置参数
    esp_err_t err = uart_param_config(m_uart_id, &uart_config);

    if (err != ESP_OK) {
        return false;
    }

    err = uart_set_pin(m_uart_id, m_tx_pin, m_rx_pin, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE); // 配置引脚

    if (err != ESP_OK) {
        return false;
    }

    const int uart_rx_buffer_size = 1024; // 接受缓冲区
    const int uart_tx_buffer_size = 1024; // 发送缓冲区

    err = uart_driver_install(m_uart_id, uart_rx_buffer_size, uart_tx_buffer_size, 0, NULL, 0);

    if (err != ESP_OK) {
        return false;
    }

    success = true;
    return true;
}

// 发送
bool Uart::write(const uint8_t* data, int data_len) {

    if (!success)
    return false;

    int len;
    len = uart_write_bytes(m_uart_id, (const uint8_t*)data, data_len);

    if (len == -1 or (len != data_len))
        return false;
    else
        return true;
}

// 发送float
bool Uart::write(float data) {
    uint8_t bytes[sizeof(float)];
    std::memcpy(bytes, &data, sizeof(float));
        
    write(bytes, sizeof(float));

    return true;
}

// 重构的类似printf的串口打印功能
bool Uart::printf(const char* fmt, ...) {
    if (!success) {
        return false;
    }

    // 初始化可变参数列表
    va_list args;
    va_start(args, fmt);

    /* 
    第一次调用 vsnprintf 来确定需要的缓冲区大小
    通过传入 nullptr 和 0 大小，vsnprintf 不会写入任何东西，
    但会返回格式化后字符串的长度（不包括末尾的 '\0'）
    注意：需要一个 va_list 的副本，因为 vsnprintf 可能会修改它 
    */
    va_list args_copy;
    va_copy(args_copy, args);
    int len = vsnprintf(nullptr, 0, fmt, args_copy);
    va_end(args_copy);

    if (len < 0) {
        // 如果出错，清理并返回失败
        va_end(args);
        return false;
    }

    /* 
    创建一个足够大的缓冲区来存放格式化后的字符串
    大小为 len + 1，为末尾的 '\0' 留出空间 
    */
    std::vector<char> buffer(len + 1);

    // 第二次调用 vsnprintf，这次真正地将内容写入缓冲区
    vsnprintf(buffer.data(), buffer.size(), fmt, args);

    va_end(args); // 清理可变参数列表

    return write(reinterpret_cast<const uint8_t*>(buffer.data()), len); // 发送
}

/**
 * @brief 从串口接受缓冲区接受指定的字节数，并设置超时时间
 * 
 * @param 缓冲区
 * @param 读取字节长度
 * @param 超时时间（FreeRTOS的Tick）
 * 
 * @return （-1）错误，（其他）在超时时间内读取到的字节数
 */
int Uart::read(void* dataBuff, size_t data_len, TickType_t timeout) {
    if (!success) return -1;

    return uart_read_bytes(m_uart_id, dataBuff, data_len, timeout);
}
