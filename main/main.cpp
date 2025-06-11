#include "main.hpp"

extern "C" void app_main(void) {
    Uart uart0(UART_NUM_0, 1, 3);
    Adc adc1(ADC_UNIT_1, ADC_CHANNEL_6);
    Motor M1(LEDC_TIMER_1, LEDC_CHANNEL_0, 13, 400);
    MPU9250 mpu9250(I2C_NUM_0, 32, 33);

    while (1) {
        uint8_t data;
        if (mpu9250.connective())
            data = 1;
        else
            data = 0;

        uart0.write(&data, 1);
        delay_ms(10);
    }

}
