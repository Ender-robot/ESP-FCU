#ifndef ICM20948_HPP
#define ICM20948_HPP

#include <cmath>
#include "i2c.hpp"
#include "system.hpp"
#include "struct.hpp"
#include "esp_log.h"

/**
 * @brief 九轴IMU ICM20948
 * 
 * @param driver IIC驱动类
 * 
 * @note ICM的地址有两种0x68和0x69，这里默认为0x68，请注意你的传感器配置模式
 *       若是0x69请修改icm20948.hpp内的定义
 */
class ICM20948 {
    public:
        ICM20948(I2C& driver);
        ~ICM20948();

        bool wakeUp();
        bool connective();

    private:
        static constexpr const char* TAG = "ICM20948"; // 日志标签
        
        enum REG : uint8_t {
            ICM20948_ADDR = 0x68, // 设备地址
            PWR_MGMT_1    = 0x06, // 设备电源及时钟管理寄存器
            WHO_AM_I      = 0x00, // 设备识别寄存器
            REG_BANK_SEL  = 0x7F, // 组选择寄存器
        };

        I2C& i2c;
        bool success;
};

#endif
