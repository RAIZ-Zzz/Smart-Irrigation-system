idf_component_register(SRCS "mqtt.c"
                    INCLUDE_DIRS "include"
                    REQUIRES
                        mqtt_client
                        esp_tls
                    )