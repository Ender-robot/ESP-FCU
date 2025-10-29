#include "i2c.hpp"

I2C::I2C(i2c_port_t i2c_id, int sda, int scl)
    : m_i2c_id(i2c_id), m_sda(sda), m_scl(scl), success(false) {
    }

I2C::~I2C() {
    if (success)
        i2c_driver_delete(m_i2c_id);
}

bool I2C::init() {
    esp_err_t err;

    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = m_sda,         // 配置 SDA 的 GPIO
        .scl_io_num = m_scl,         // 配置 SCL 的 GPIO
        .sda_pullup_en = GPIO_PULLUP_DISABLE,
        .scl_pullup_en = GPIO_PULLUP_DISABLE,
        .master = {
            .clk_speed = 400000    // 为项目选择频率
        },  
        .clk_flags = 0,            // 可选项，可以使用 I2C_SCLK_SRC_FLAG_* 标志来选择 I2C 源时钟
    };

    err = i2c_param_config(m_i2c_id, &conf); // 设置外设
    if (err != ESP_OK)
        return false;

    err = i2c_driver_install(m_i2c_id, I2C_MODE_MASTER, 0, 0, 0); // 安装驱动
    if (err != ESP_OK)
        return false;

    success = true;
    return true;
}

bool I2C::write_byte_to_mem(uint8_t addr, uint8_t mem_addr, uint8_t data) {
    i2c_cmd_handle_t cmd_link = i2c_cmd_link_create(); // 创建命令链
    i2c_master_start(cmd_link); // 起始位
    i2c_master_write_byte(cmd_link, (addr << 1 | I2C_MASTER_WRITE), true); // 从机地址(左移一位或上0表示写)
    i2c_master_write_byte(cmd_link, mem_addr, true); // 寄存器地址
    i2c_master_write_byte(cmd_link, data, true); // 数据字节
    i2c_master_stop(cmd_link); // 停止位

    esp_err_t err = i2c_master_cmd_begin(m_i2c_id, cmd_link, pdMS_TO_TICKS(10)); // 发送指令链
    i2c_cmd_link_delete(cmd_link); // 清除指令链内存
    if (err != ESP_OK) {
        return false;
    }
    
    return true;
}

/**
 * @brief 从I2C从机的指定内存地址开始，连续读取多个字节 (突发读取)
 * 
 * @param addr      从机设备地址
 * @param mem_addr  要开始读取的寄存器地址
 * @param bytes_buf  存放读取数据的缓冲区指针
 * @param len       要读取的字节数
 * @return true     读取成功
 * @return false    读取失败
 */
bool I2C::read_bytes_from_mem(uint8_t addr, uint8_t mem_addr, uint8_t* bytes_buf, size_t len) {
    if (len == 0) { // 读取一个字节直接返回成功
        return true;
    }

    i2c_cmd_handle_t cmd_link = i2c_cmd_link_create(); // 创建命令链
    i2c_master_start(cmd_link); // 起始位
    i2c_master_write_byte(cmd_link, (addr << 1 | I2C_MASTER_WRITE), true); // 从机地址(左移一位或上1表示写)
    i2c_master_write_byte(cmd_link, mem_addr, true); // 寄存器地址
    i2c_master_start(cmd_link); // 重复起始位
    i2c_master_write_byte(cmd_link, (addr << 1 | I2C_MASTER_READ), true); //  重复从机地址(左移一位或上1表示写)
    // 连续读取
    if (len -1 ) {
        i2c_master_read(cmd_link, bytes_buf, len - 1, I2C_MASTER_ACK); // 读取前len-1个字节
    }
    i2c_master_read_byte(cmd_link, bytes_buf + len - 1, I2C_MASTER_NACK); // 读取最后1个字节
    i2c_master_stop(cmd_link); // 停止位

    esp_err_t err = i2c_master_cmd_begin(m_i2c_id, cmd_link, pdMS_TO_TICKS(50)); // 发送指令链
    i2c_cmd_link_delete(cmd_link); // 清除指令链内存
    if (err != ESP_OK) {
        return false;
    }
    
    return true;
}
