#include "main.hpp"

// 创建外设驱动
I2c i2c0(I2C_NUM_0, 21, 22);
Uart uart0(UART_NUM_0, 1, 3);
MPU9250 mpu9250(i2c0);
Flash flash;

void testTask(void *pvParameters) {
    (void) pvParameters; // 告诉编译器我知道这个没有别警告我

    Rate rate(250);
    Vec3f accelData;
    
    while (1) {
        if (mpu9250.read_gyro(accelData)) {
            uart0.printf("%.2f %.2f %.2f\r\n", accelData.x, accelData.y, accelData.z);
        }
        else uart0.printf("ERORR: no data !\n");

        rate.sleep();
    }
}

void testTask1(void *pvParameters) {
    (void) pvParameters; // 告诉编译器我知道这个没有别警告我

    Rate rate(250);
    Vec3f cailData;

    mpu9250.cail_gyro(cailData);
    flash.saveAsBlob("CailGyroBiasData", &cailData, sizeof(Vec3f));

    UBaseType_t uxHighWaterMark = uxTaskGetStackHighWaterMark(NULL); // 传入 NULL 表示查询当前任务
    printf("testTask1 stack high water mark: %u words\n", uxHighWaterMark);

    while (1) delay(1);
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
    // xTaskCreate(testTask1, "testTask1", 4096, NULL, 1, NULL);
}
