#include "mpu9250.hpp"

MPU9250::MPU9250(i2c_port_t i2c_id, int sda, int scl)
    : m_i2c_id(i2c_id), m_sda(sda), m_scl(scl), i2c(m_i2c_id, m_sda, m_scl) {

        /*---初始化---*/
        i2c.write_byte_to_mem(MPU_ADDR, PWR_MGMT_1, 0x01); // 唤醒MPU6500并设置时钟

        /*设置*/
        i2c.write_byte_to_mem(MPU_ADDR, SMPLRT_DIV, 0x09); // 设置MPU6500采样率
        i2c.write_byte_to_mem(MPU_ADDR, GYRO_CONFIG, 0x08); // 设置陀螺仪量程，该量程下LSB = 65.534
        i2c.write_byte_to_mem(MPU_ADDR, CONFIG, 0x03); // 设置陀螺仪高频滤波器
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

bool MPU9250::read_gyro(float& x, float& y, float& z) {
    int ADDR = GYRO_XOUT_H;

    float data_bufs[3];
    for (int i = 0; i <= 2; i++) {
        uint8_t hightByte, lowByte; 
        i2c.read_byte_from_mem(MPU_ADDR, ADDR, hightByte); // 读取一个轴的高字节
        i2c.read_byte_from_mem(MPU_ADDR, ++ADDR, lowByte); // 读取一个轴的低字节
        data_bufs[i] = UINT8_2_INT16(hightByte, lowByte) / 65.534; // 存入数组，并转换为物理单位角度
        ADDR++;
    }

    x = data_bufs[0];
    y = data_bufs[1];
    z = data_bufs[2];

    return true;
}
