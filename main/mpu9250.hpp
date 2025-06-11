#ifndef MPU9250_HPP
#define MPU9250_HPP

#include "i2c.hpp"

/*----寄存器----*/
#define MPU_ADDR          0x68
#define WHO_AM_I_REG      0x75 // Should return 0x71
#define PWR_MGMT_1        0x6B
#define SMPLRT_DIV        0x19
#define CONFIG            0x1A
#define GYRO_CONFIG       0x1B
#define ACCEL_CONFIG      0x1C
#define ACCEL_CONFIG_2    0x1D
#define INT_PIN_CFG       0x37
#define USER_CTRL         0x6A

class MPU9250 {
    public:
        MPU9250(i2c_port_t i2c_id, int sda, int scl);
        ~MPU9250();

        bool connective();

    private:
        i2c_port_t m_i2c_id;
        int m_sda;
        int m_scl;
        I2c i2c;
};

#endif
