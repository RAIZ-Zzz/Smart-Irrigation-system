#include <stdio.h>
#include <my_wifi.h>
#include <my_mqtt.h>

// WIFI INIT
#define SSID "NIRVANA"
#define PASSWORD "NIRVANAFOREVER"

//MQTT INIT
#define MQTT_ADDRESS "mqtt://broker-cn.emqx.io"
#define MQTT_CLIENTID "mqtt_esp3220251202"
#define MQTT_USERNAME "esp32username"
#define MQTT_PASSWORD "esp32password"
#define TAG "mqtt"

#define MQTT_TOPIC_ESP32 "/topic/esp32_1123"
#define MQTT_TOPIC_SUBSCRIBE "/topic/mqttx_1123"

void app_main(void)
{
    wifi_init(SSID, PASSWORD);
}
