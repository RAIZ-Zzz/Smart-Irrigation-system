#include <stdio.h>
#include <my_wifi.h>
#include <my_mqtt.h>
#include <my_gpio.h>
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

//Valve GPIO setting

#define VALVE_GPIO GPIO_NUM_9

void led_control(void* Param)
{
    gpio_num_t led_pin = (gpio_num_t)(int)Param;
    while(1)
    {
        xSemaphoreTake(s_mqtt_publish_sem, portMAX_DELAY);
        gpio_control(led_pin,1);
        vTaskDelay(pdMS_TO_TICKS(1000));
        gpio_control(led_pin,0);
    }
}

void app_main(void)
{
    // Semaphore init
    s_wifi_connect_sem = xSemaphoreCreateBinary();//do something after connect wifi
    s_mqtt_publish_sem = xSemaphoreCreateBinary();//give a semaphore when receive information from server
    s_sntp_init_sem = xSemaphoreCreateBinary();//sntp reconnect semaphore
    
    wifi_init(SSID, PASSWORD);
    
    xSemaphoreTake(s_wifi_connect_sem,portMAX_DELAY);
    mqtt_start(MQTT_TOPIC_SUBSCRIBE, MQTT_ADDRESS, MQTT_CLIENTID, MQTT_USERNAME, MQTT_PASSWORD);//mqtt init
    time_sync_init();
    //valve GPIO pin init
    gpio_init(VALVE_GPIO);
    
    xTaskCreate(time_info_send,"time info publish",4096,(void*)MQTT_TOPIC_PUBLISH,3,NULL);
    xTaskCreate(sntp_reconnect,"sntp reconnect",4096,NULL,3,NULL);
    xTaskCreate(led_control,"sntp reconnect",4096,(void*)VALVE_GPIO,3,NULL);
    
}

