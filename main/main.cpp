#include "main.hpp"

// 创建驱动类
I2C i2c0(I2C_NUM_0, 15, 16);
Uart uart1(UART_NUM_0, 40, 41);
Flash flash;
GPIO led_debug(GPIO_NUM_17, GPIO_MODE_OUTPUT, 1);

MPU9250 mpu9250(i2c0);
COMM comm(uart1);

void testTask(void *pvParameters) {
    (void) pvParameters; // 告诉编译器我知道这个没有别警告我

    const int hz = 250;
    const float dt = 1.0f / hz;
    Rate rate(hz); // 简单频率控制（非精确）
    Vec3f accelData, gyroData, gyroBias, accelBias, accelGain, atti;
    
    // 读取传感器校准值（须提前校准并存入NVS）
    size_t len = sizeof(Vec3f);
    flash.readAsBlob("GyroBias", &gyroBias, &len); 
    flash.readAsBlob("AccelBias", &accelBias, &len); 
    flash.readAsBlob("AccelGain", &accelGain, &len); 

    // 创建姿态角估计对象
    AHRS ahrs(gyroBias, accelBias, accelGain);

    while (1) {
        // 读取传感器
        mpu9250.read_gyro(gyroData);
        mpu9250.read_accel(accelData);

        // 估计
        atti = ahrs.attiEst(gyroData, accelData, dt, CF{});

        // 输出
        uart1.printf("%f %f %f\r\n", atti.x, atti.y, atti.z);

        rate.sleep();
    }
}

void testTask1(void *pvParameters) {
    (void) pvParameters; // 告诉编译器我知道这个没有别警告我

    Rate rate(0.5);
    RoBoFeedBack pack = { // 随便发个输测试一下，不需要手动初始化或者定义包头，结构体定义时内部就以初始化
        .roll = 1.0f
    };

    while (1) {
        comm.uartSendPack(pack);
        rate.sleep();
    }
}

void testTask2(void *pvParameters) {
    (void) pvParameters; // 告诉编译器我知道这个没有别警告我

    Rate rate(50);
    RoBoCmd pack; // 数据包缓冲区
    bool ret;

    while (1)
    {
        ret = comm.uartRecePack(pack);
        if (ret) ESP_LOGI("RxPack", "val1: %.2f, mode1: %d", pack.val1, pack.mode1);
        rate.sleep();
    }
}

void testTask3(void *pvParameters) {
    (void) pvParameters; // 告诉编译器我知道这个没有别警告我

    while (1)
    {
        led_debug.write(0);
        delay_ms(200);
        led_debug.write(1);
        delay_ms(200);
    }
    

}

void testTask4(void *pvParameters) {
    (void) pvParameters; // 告诉编译器我知道这个没有别警告我

    // I2C设备地址
    uint8_t ICM20948_ADDR = 0x68;

    // 寄存器地址
    uint8_t REG_BANK_SEL  = 0x7F;
    uint8_t PWR_MGMT_1_    = 0x06;
    uint8_t WHO_AM_I      = 0x00;

    uint8_t data = 200;

    i2c0.write_byte_to_mem(ICM20948_ADDR, PWR_MGMT_1_, 0x01);
    i2c0.write_byte_to_mem(ICM20948_ADDR, REG_BANK_SEL, 0x00);
    i2c0.read_bytes_from_mem(ICM20948_ADDR, WHO_AM_I, &data, 1);

    while (1)
    {
        ESP_LOGI("Test4", "%d", data);
        delay_ms(500);
    }
    
}

extern "C" void app_main(void) {
    // 初始化硬件外设驱动
    i2c0.init();

    uart1.init();

    led_debug.init();

    if (flash.init()) uart1.printf("init");
    
    if (!mpu9250.init()) {
        uart1.printf("\nERORR: fail to connect mpu !\n");
    }

    delay_ms(500);

    // 测试一个demo时，应注释掉另一个
    // xTaskCreate(testTask, "testTask", 4096, NULL, 1, NULL);
    // xTaskCreate(testTask1, "testTask1", 4096, NULL, 1, NULL);
    // xTaskCreate(testTask2, "testTask2", 4096, NULL, 1, NULL);
    xTaskCreate(testTask3, "testTask3", 4096, NULL, 1, NULL);
    xTaskCreate(testTask4, "testTask4", 4096, NULL, 1, NULL);

    while (1)
    {
        delay(102400);
    }
    
}
