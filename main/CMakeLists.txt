idf_component_register(SRCS "ahrs.cpp" "mpu9250.cpp" "main.cpp"
                    PRIV_REQUIRES freertos hardware 
                    INCLUDE_DIRS ".")
