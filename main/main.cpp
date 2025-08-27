#include "main.hpp"

// 创建外设驱动
I2c i2c0(I2C_NUM_0, 21, 22);
Uart uart0(UART_NUM_0, 1, 3);
MPU9250 mpu9250(i2c0);
Flash flash;

void testTask(void *pvParameters) {
    (void) pvParameters; // 告诉编译器我知道这个没有别警告我

    const int hz = 250;
    const float dt = 1.0f / hz;
    Rate rate(hz); // 简单频率控制（非精确）
    Vec3f accelData, gyroData, gyroBias, accelBias, accelGain, atti;
    
    // 读取传感器校准值
    size_t len = sizeof(Vec3f);
    flash.readAsBlob("GyroBias", &gyroBias, &len); 
    flash.readAsBlob("AccelBias", &accelBias, &len); 
    bool ret = flash.readAsBlob("AccelGain", &accelGain, &len); 
    ESP_LOGI("Test", "gyro bias: %.2f %.2f %.2f", gyroBias.x, gyroBias.y, gyroBias.z);
    ESP_LOGI("Test", "accel bias: %.2f %.2f %.2f", accelBias.x, accelBias.y, accelBias.z);
    ESP_LOGI("Test", "accel gain: %.2f %.2f %.2f", accelGain.x, accelGain.y, accelGain.z);
    // 创建姿态角估计对象
    AHRS ahrs(gyroBias, accelBias, accelGain);

    while (1) {
        // 读取传感器
        mpu9250.read_gyro(gyroData);
        mpu9250.read_accel(accelData);

        // 估计
        atti = ahrs.attiEst(gyroData, accelData, dt, CF{});

        // 输出
        uart0.printf("%f %f %f\r\n", atti.x, atti.y, atti.z);

        rate.sleep();
    }
}

extern "C" void app_main(void) {
    // 初始化硬件外设驱动
    i2c0.init();
    uart0.init();
    bool ret = flash.init();
    if (ret) uart0.printf("init");
    
    if (!mpu9250.init()) {
        uart0.printf("\nERORR: fail to connect mpu !\n");
        return ;
    }

    delay_ms(500);

    xTaskCreate(testTask, "testTask", 4096, NULL, 1, NULL);
}
