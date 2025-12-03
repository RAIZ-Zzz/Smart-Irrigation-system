#ifndef MY_WIFI_H  
#define MY_WIFI_H
#include <stdio.h>
#include "esp_err.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h" 


/*
Directly use the function "ifi_init(const char *SSID, const char *PASSWORD)" to initiate wifi connection:
example:
#define SSID "SSID of your wifi"
#define PASSWORD "PASSWORD of our wifi"

void app_main(void)
{
    wifi_init(SSID, PASSWORD);
    
}
Then esp32 will connect to wifi successfully.

*/
extern SemaphoreHandle_t s_wifi_connect_sem;
extern SemaphoreHandle_t s_sntp_init_sem;
void wifi_init(const char *SSID, const char *PASSWORD);

#endif