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


void time_info_send(void* Param)//if receive message do time report
{   
    while(1)
    {
        xSemaphoreTake(s_mqtt_publish_sem,portMAX_DELAY);
        int i = 0;
        while(i<5)
        {
            time_report(MQTT_TOPIC_PUBLISH);  
            i++;
            vTaskDelay(pdMS_TO_TICKS(1000));
        }
        ESP_LOGI("TASK", "Time report task completed");
    }
    

}

void sntp_connect(void* Param)
{
    while(1)
    {
        xSemaphoreTake(s_sntp_init_sem, portMAX_DELAY);
        vTaskDelay(pdMS_TO_TICKS(2000));
        time_sync_init();
    }
}

void app_main(void)
{
    s_wifi_connect_sem = xSemaphoreCreateBinary();
    s_mqtt_publish_sem = xSemaphoreCreateBinary();
    s_sntp_init_sem = xSemaphoreCreateBinary();
    wifi_init(SSID, PASSWORD);
    xSemaphoreTake(s_wifi_connect_sem,portMAX_DELAY);
    mqtt_start(MQTT_TOPIC_SUBSCRIBE, MQTT_ADDRESS, MQTT_CLIENTID, MQTT_USERNAME, MQTT_PASSWORD);//mqtt init

    xTaskCreate(time_info_send,"time info publish",4096,NULL,3,NULL);
    xTaskCreate(sntp_connect,"sntp reconnect",4096,NULL,3,NULL);

    
}

