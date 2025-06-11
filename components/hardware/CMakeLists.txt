idf_component_register(SRCS "motor_control.cpp" "i2c.cpp" "uart.cpp" "pwm.cpp" "adc.cpp" "system.cpp"
                    REQUIRES driver freertos esp_adc 
                    INCLUDE_DIRS ".")
