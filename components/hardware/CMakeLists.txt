idf_component_register(SRCS "flash.cpp" "motor_control.cpp" "i2c.cpp" "uart.cpp" "pwm.cpp" "adc.cpp" "system.cpp" "flash.cpp"
                    REQUIRES driver freertos esp_adc nvs_flash
                    INCLUDE_DIRS ".")
