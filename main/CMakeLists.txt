idf_component_register(SRCS "datapack.cpp" "ahrs.cpp" "main.cpp"
                    PRIV_REQUIRES freertos hardware interface
                    INCLUDE_DIRS ".")
