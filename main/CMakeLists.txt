idf_component_register(SRCS "mpu9250.cpp" "main.cpp"
                    PRIV_REQUIRES freertos hardware 
                    INCLUDE_DIRS ".")
