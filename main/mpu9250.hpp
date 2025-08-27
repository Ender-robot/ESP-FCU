#ifndef MPU9250_HPP
#define MPU9250_HPP

#include <cmath>
#include "i2c.hpp"
#include "system.hpp"
#include "struct.hpp"
#include "esp_log.h"

/*----寄存器----*/
#define MPU_ADDR          0x68 // MPU9250的默认地址
#define WHO_AM_I_REG      0x75 // Should return 0x71
#define PWR_MGMT_1        0x6B
#define SMPLRT_DIV        0x19
#define CONFIG            0x1A
#define GYRO_CONFIG       0x1B
#define ACCEL_CONFIG      0x1C
#define ACCEL_CONFIG_2    0x1D
#define INT_PIN_CFG       0x37
#define USER_CTRL         0x6A

/*以下为数据位寄存器地址，高位在前，因为是连续的6位所以都是只有X的高位*/
#define ACCEL_XOUT_H      0x3B
#define GYRO_XOUT_H       0x43
#define TEMP_OUT_H        0x41

static const char *TAG = "MPU9250";

/**
 * @brief 初始化MPU9250
 * 
 * @param i2c_id  使用I2C总线ID
 * @param sda     SDA的IO
 * @param scl     SCL的IO
*/
class MPU9250 {
    public:
        MPU9250(I2c driver);
        ~MPU9250();

        bool init(); // 初始化
        bool connective(); // 检测连接是否成功
        bool read_gyro(Vec3f& data); // 读陀螺仪
        bool cail_gyro(Vec3f& cailData); // 校准陀螺仪
        bool read_accel(Vec3f& data); // 读加速度计
        bool cail_accel(Vec3f& cailBiasData, Vec3f& cailGainData); // 校准加速度计

    private:
        I2c i2c;
        bool success;
};

#endif
