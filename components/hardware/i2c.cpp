#include "i2c.hpp"

I2c::I2c(i2c_port_t i2c_id, int sda, int scl)
    : m_i2c_id(i2c_id), success(false) {
        esp_err_t err;

        i2c_config_t conf = {
            .mode = I2C_MODE_MASTER,
            .sda_io_num = sda,         // 配置 SDA 的 GPIO
            .scl_io_num = scl,         // 配置 SCL 的 GPIO
            .sda_pullup_en = GPIO_PULLUP_DISABLE,
            .scl_pullup_en = GPIO_PULLUP_DISABLE,
            .master = {
                .clk_speed = 400000    // 为项目选择频率
            },  
            .clk_flags = 0,            // 可选项，可以使用 I2C_SCLK_SRC_FLAG_* 标志来选择 I2C 源时钟
        };

        err = i2c_param_config(m_i2c_id, &conf); // 设置外设
        if (err != ESP_OK)
            return;

        err = i2c_driver_install(m_i2c_id, I2C_MODE_MASTER, 0, 0, 0); // 安装驱动
        if (err != ESP_OK)
            return;

        success = true;
    }

I2c::~I2c() {
    if (success)
        i2c_driver_delete(m_i2c_id);
}

bool I2c::write_byte_to_mem(uint8_t addr, uint8_t mem_addr, uint8_t data) {
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

bool I2c::read_byte_from_mem(uint8_t addr, uint8_t mem_addr, uint8_t& data_byte) {
    i2c_cmd_handle_t cmd_link = i2c_cmd_link_create(); // 创建命令链
    i2c_master_start(cmd_link); // 起始位
    i2c_master_write_byte(cmd_link, (addr << 1 | I2C_MASTER_WRITE), true); // 从机地址(左移一位或上1表示写)
    i2c_master_write_byte(cmd_link, mem_addr, true); // 寄存器地址
    i2c_master_start(cmd_link); // 重复起始位
    i2c_master_write_byte(cmd_link, (addr << 1 | I2C_MASTER_READ), true); //  重复从机地址(左移一位或上1表示写)
    i2c_master_read_byte(cmd_link, &data_byte, I2C_MASTER_NACK); // 读取一个字节
    i2c_master_stop(cmd_link); // 停止位

    esp_err_t err = i2c_master_cmd_begin(m_i2c_id, cmd_link, pdMS_TO_TICKS(10)); // 发送指令链
    i2c_cmd_link_delete(cmd_link); // 清除指令链内存
    if (err != ESP_OK) {
        return false;
    }
    
    return true;
}
