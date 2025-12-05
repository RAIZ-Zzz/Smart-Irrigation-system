idf_component_register(SRCS "SmartIrrigation.c"
                    INCLUDE_DIRS "."
                    REQUIRES my_wifi my_mqtt my_gpio)