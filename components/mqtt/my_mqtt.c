#include <stdio.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

#include "mqtt_client.h"

#include "esp_tls.h"
#include "esp_sntp.h"

#define TAG "mqtt"

static esp_mqtt_client_handle_t mqtt_handle = NULL;

static char strftime_buf[64];

void mqtt_event_callback( void* event_handler_arg, esp_event_base_t event_base, int32_t event_id, void* event_data)
{
    char *target_topic = (char *)event_handler_arg;
    esp_mqtt_event_handle_t data = (esp_mqtt_event_handle_t)event_data;
    switch (event_id)
    {
    case MQTT_EVENT_CONNECTED:
        ESP_LOGI(TAG,"MQTT connected");
        esp_mqtt_client_subscribe_single(mqtt_handle, target_topic,1);
        break;

    case MQTT_EVENT_DISCONNECTED:
        ESP_LOGI(TAG,"MQTT disconnected");
        break;
    
    case MQTT_EVENT_PUBLISHED:
        ESP_LOGI(TAG,"MQTT published ack");
        break;
    
    case MQTT_EVENT_SUBSCRIBED:
        ESP_LOGI(TAG,"MQTT subscribed ack");
        break;
    
    case MQTT_EVENT_DATA:
        
        ESP_LOGI(TAG,"topic ->%.*s",data->topic_len,data->topic);
        ESP_LOGI(TAG,"payload -> %.*s",data->data_len,data->data);
        break;
    
    default:
        break;
    }
}

void mqtt_start(const char* MQTT_TOPIC2,const char* MQTT_ADDRESS,const char* MQTT_CLIENTID,const char* MQTT_USERNAME,const char* MQTT_PASSWORD)
{
    esp_mqtt_client_config_t mqtt_cfg = {0};
    mqtt_cfg.broker.address.uri = MQTT_ADDRESS;
    mqtt_cfg.broker.address.port = 1883;
    mqtt_cfg.credentials.client_id = MQTT_CLIENTID;
    mqtt_cfg.credentials.username = MQTT_USERNAME;
    mqtt_cfg.credentials.authentication.password = MQTT_PASSWORD;
    mqtt_handle = esp_mqtt_client_init(&mqtt_cfg);
    
    esp_mqtt_client_register_event(mqtt_handle, ESP_EVENT_ANY_ID, mqtt_event_callback, (void *)MQTT_TOPIC2);

    esp_mqtt_client_start(mqtt_handle);

}    

