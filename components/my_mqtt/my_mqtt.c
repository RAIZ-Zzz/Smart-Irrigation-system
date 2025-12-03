#include <stdio.h>
#include <string.h>
#include <my_wifi.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

#include "mqtt_client.h"

#include "esp_log.h"
#include "esp_tls.h"
#include "esp_sntp.h"

#include <time.h>
#include <sys/time.h>

#define TAG "mqtt"

static bool sntp_initialized = false;

static esp_mqtt_client_handle_t mqtt_handle = NULL;

static char strftime_buf[64];

SemaphoreHandle_t s_mqtt_publish_sem = NULL;


void mqtt_event_callback( void* event_handler_arg, esp_event_base_t event_base, int32_t event_id, void* event_data)
{
    char *target_topic = (char *)event_handler_arg;
    esp_mqtt_event_handle_t data = (esp_mqtt_event_handle_t)event_data;
    switch (event_id)
    {
    case MQTT_EVENT_CONNECTED:
        ESP_LOGI(TAG,"MQTT connected");
        esp_mqtt_client_subscribe_single(mqtt_handle, target_topic,1);
        xSemaphoreGive(s_sntp_init_sem);
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
        ESP_LOGI(TAG,"topic -> \n %.*s",data->topic_len,data->topic);
        ESP_LOGI(TAG,"payload -> \n %.*s",data->data_len,data->data);
        xSemaphoreGive(s_mqtt_publish_sem); 
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
    
    if (mqtt_handle == NULL) {
        ESP_LOGE(TAG, "MQTT client init failed");
        return;
    }
    
    esp_mqtt_client_register_event(mqtt_handle, ESP_EVENT_ANY_ID, mqtt_event_callback, (void *)MQTT_TOPIC2);

    esp_err_t ret = esp_mqtt_client_start(mqtt_handle);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "MQTT client start failed: %s", esp_err_to_name(ret));
        esp_mqtt_client_destroy(mqtt_handle);
        mqtt_handle = NULL;
    }

}    

void mqtt_publish(const char* MESSAGE,const char* MQTT_TOPIC1)
{
    char payload[256];
    int len = snprintf(payload, sizeof(payload), "{ \"message\": \"%s\" }", MESSAGE);
    if (len < 0 || len >= sizeof(payload)) {
        ESP_LOGW(TAG, "MQTT payload truncated");
    }
    esp_mqtt_client_publish(mqtt_handle,MQTT_TOPIC1,payload,strlen(payload),1,0);
}

/*==================================TIME SYNCHRONIZE=================================*/

void notify(struct timeval* t) 
{
    ESP_LOGI("SNTP","synchronized");
    sntp_initialized = true;
}

void setTimezone() 
{  
    setenv("TZ", "CST-8", 1);
    tzset();
}

void initSNTP() 
{
    sntp_set_sync_interval(60 * 60 * 1000UL); 
    sntp_set_time_sync_notification_cb(notify);
    esp_sntp_setoperatingmode(ESP_SNTP_OPMODE_POLL);
    esp_sntp_setservername(0, "ntp.aliyun.com");
    esp_sntp_init();
    setTimezone();
}

void wait4SNTP()
{   
    ESP_LOGI("SNTP", "Waiting for synchronization...");   
    while (sntp_get_sync_status() != SNTP_SYNC_STATUS_COMPLETED) 
    {
        ESP_LOGI("SNTP", "waiting... ");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void updateTime() 
{
    time_t now;
    struct tm timeinfo;
    time(&now);
    localtime_r(&now, &timeinfo);
    strftime(strftime_buf, sizeof(strftime_buf), "{\"Time\":\"%A, %B %d %Y %H:%M:%S\"}", &timeinfo);

}

void time_sync_init()
{
    if (esp_sntp_enabled()) {
        ESP_LOGI(TAG, "SNTP was running, stopping it first...");
        esp_sntp_stop();
    }
    ESP_LOGI("SNTP", "Initializing SNTP...");
    initSNTP();
    wait4SNTP();  
    ESP_LOGI("SNTP", "Time sync initialization completed");
}

void time_report(const char*MQTT_TOPIC1)
{
    updateTime(); 
    esp_mqtt_client_publish(mqtt_handle,MQTT_TOPIC1,strftime_buf,strlen(strftime_buf),1,0);
}

