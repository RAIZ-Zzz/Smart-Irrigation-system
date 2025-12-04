#include <stdio.h>
#include <my_wifi.h>
#include <my_mqtt.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h" 
#include "esp_log.h"
// WIFI INIT
#define SSID "NIRVANA"
#define PASSWORD "NIRVANAFOREVER"

//MQTT INIT
#define MQTT_ADDRESS "mqtt://broker-cn.emqx.io"
#define MQTT_CLIENTID "mqtt_esp3220251202"
#define MQTT_USERNAME "esp32username"
#define MQTT_PASSWORD "esp32password"


#define MQTT_TOPIC_PUBLISH "/topic/esp32_1123"
#define MQTT_TOPIC_SUBSCRIBE "/topic/mqttx_1123"


void app_main(void)
{
    s_wifi_connect_sem = xSemaphoreCreateBinary();//do something after connect wifi
    s_mqtt_publish_sem = xSemaphoreCreateBinary();//give a semaphore when receive information from server
    s_sntp_init_sem = xSemaphoreCreateBinary();//sntp reconnect semaphore
    wifi_init(SSID, PASSWORD);
    xSemaphoreTake(s_wifi_connect_sem,portMAX_DELAY);
    mqtt_start(MQTT_TOPIC_SUBSCRIBE, MQTT_ADDRESS, MQTT_CLIENTID, MQTT_USERNAME, MQTT_PASSWORD);//mqtt init

    xTaskCreate(time_info_send,"time info publish",2048,(void*)MQTT_TOPIC_PUBLISH,3,NULL);
    xTaskCreate(sntp_reconnect,"sntp reconnect",2048,NULL,3,NULL);

    
}

