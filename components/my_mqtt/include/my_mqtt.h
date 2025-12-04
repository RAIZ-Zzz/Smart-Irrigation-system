#ifndef MY_MQTT_H
#define MY_MQTT_H

extern SemaphoreHandle_t s_mqtt_publish_sem;

void mqtt_start(const char* MQTT_TOPIC2,const char* MQTT_ADDRESS,const char* MQTT_CLIENTID,const char* MQTT_USERNAME,const char* MQTT_PASSWORD);//mqtt init

void mqtt_publish(char* MESSAGE, char* MQTT_TOPIC1);//mqtt send json message 

void time_sync_init();//time sync initialize

void time_report(const char*MQTT_TOPIC1);// time report

void time_info_send(void* Param);

void sntp_reconnect(void* Param);

#endif
