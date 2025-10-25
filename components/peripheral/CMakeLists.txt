idf_component_register(SRCS "pin.cpp" "flash.cpp" "i2c.cpp" "uart.cpp" "system.cpp"
                       REQUIRES driver freertos esp_adc nvs_flash
                       INCLUDE_DIRS ".")
