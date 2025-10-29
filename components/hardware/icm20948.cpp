#include "icm20948.hpp"

ICM20948::ICM20948(I2C& driver) 
    : i2c(driver), success(false) {

}

ICM20948::~ICM20948() {

}

/**
 * @brief 唤醒传感器
 */
bool ICM20948::wakeUp() {
    bool ret = i2c.write_byte_to_mem(ICM20948_ADDR, PWR_MGMT_1, 0x01);
    return ret;
}

/**
 * @brief 检测MPU20948连接性
 */
bool ICM20948::connective() {
    uint8_t data = 0x00;
    i2c.read_bytes_from_mem(ICM20948_ADDR, WHO_AM_I, &data, 1);
    if (data == 0xEA) return true;
    else return false;
}
