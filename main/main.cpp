#include "main.hpp"

extern "C" void app_main(void) {
    Uart uart0(UART_NUM_0, 1, 3);
    Adc adc1(ADC_UNIT_1, ADC_CHANNEL_6);
    Motor M1(LEDC_TIMER_1, LEDC_CHANNEL_0, 13, 400);
    MPU9250 mpu9250(I2C_NUM_0, 32, 33);

    while (1) {
        float x  = 0, y = 0, z = 0;

        if (mpu9250.connective())
            mpu9250.read_gyro(x, y, z);

        uart0.writeAsASCII(x);
        uart0.writeAsASCII(",");
        uart0.writeAsASCII(y);
        uart0.writeAsASCII(",");
        uart0.writeAsASCII(z);
        uart0.writeAsASCII("\n\r");

        delay_ms(10);
    }

}
