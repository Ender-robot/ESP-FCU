#ifndef I2C_HPP
#define I2C_HPP

#include "driver/i2c.h"

class I2c {
    public:
        I2c (i2c_port_t i2c_id, int sda, int scl);
        ~I2c ();

        bool init();
        bool write_byte_to_mem(uint8_t addr, uint8_t mem_addr, uint8_t data); // 向指定寄存器写一个字节
        bool read_bytes_from_mem(uint8_t addr, uint8_t mem_addr, uint8_t* bytes_buf, size_t len); // 从指定寄存器开始读取len个字节

    private:
        i2c_port_t m_i2c_id;
        int m_sda;
        int m_scl;
        bool success;
};

#endif
