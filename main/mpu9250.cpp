#include "mpu9250.hpp"

MPU9250::MPU9250(i2c_port_t i2c_id, int sda, int scl)
    : m_i2c_id(i2c_id), m_sda(sda), m_scl(scl), i2c(m_i2c_id, m_sda, m_scl) {

    }

MPU9250::~MPU9250() {

}

bool MPU9250::connective() {
    uint8_t data_byte;
    i2c.read_byte_from_mem(MPU_ADDR, WHO_AM_I_REG, data_byte);
    if (data_byte == 0x70)
        return true;
    return false;
}
